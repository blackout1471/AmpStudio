#include "AsioAudioDevice.h"
#include "Logging/Logger.h"
#include "Utility/MESA.h"
#include "Utility/ConvolutionUtility.h"
#include "Converters/Int32MsbAudioConverter.h"

namespace AmpProcessing {
	namespace Devices {

		// This is stupid, to be able to get the current context for callbacks.
		// We have to store the object which should be used for function binding.
		AsioAudioDevice* AsioAudioDevice::s_CurrentContext = nullptr;

		AsioAudioDevice::AsioAudioDevice() : m_Buffers(), m_Callbacks(), m_Channels(), m_DriverInformation(), 
			m_AudioConverter(new Converters::Int32MsbAudioConverter())
		{
		}

		AsioAudioDevice::~AsioAudioDevice()
		{
			Close();

			delete m_Buffers;
			delete m_Channels;
		}

		bool AsioAudioDevice::Open(const std::string& deviceName)
		{
			LOG_ASSERT(deviceName != "", "Device name cannot be null");

			LOG_ASSERT(LoadDriver(deviceName), "Could not load drivername {}", deviceName);

			LOG_ASSERT(LoadDeviceDetails(), "Could not load device details");

			LOG_ASSERT(SetupBuffers(), "Could not setup buffers");

			LOG_ASSERT(SetupDeviceCallbacks(), "Could not setup callbacks for device");

			LOG_ASSERT(ASIOStart() == 0, "Could not start asio");

			LOG_ASSERT(LoadChannelInformation(), "Could not get channel information");

			return true;
		}

		bool AsioAudioDevice::Close()
		{
			LOG_ASSERT(ASIOStop() == 0, "Could not stop ASIO");

			LOG_ASSERT(ASIODisposeBuffers() == 0, "Could not dispose of asio buffers");

			return true;
		}

		const std::vector<std::string> AsioAudioDevice::GetDeviceNames()
		{
			int maxSize = 32;
			auto deviceNames = std::vector<std::string>();

			char** names = new char* [maxSize];
			for (int i = 0; i < 32; i++)
			{
				names[i] = new char[maxSize];
			}

			m_AsioDriver.getDriverNames(names, maxSize);

			for (int i = 0; i < maxSize; i++)
			{
				if (names[i][0] != -51)
					deviceNames.push_back(names[i]);
				delete[] names[i];
			}

			delete[] names;

			return deviceNames;
		}

		bool AsioAudioDevice::LoadDriver(const std::string& driverName)
		{
			char* name = _strdup(driverName.c_str());
			bool success = m_AsioDriver.loadDriver(name);
			LOG_ASSERT(success, "Could not load driver {}", name);
			delete name;

			return true;
		}

		bool AsioAudioDevice::LoadDeviceDetails()
		{
			int code = ASIOInit(&m_DriverInformation);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not initialise ASIO", code);
			if (code != 0)
				return false;

			LOG_INFO("[ASIO] Device loaded [{}]", m_DriverInformation.name);
			LOG_INFO("[ASIO] Driver version [{}]", m_DriverInformation.driverVersion);
			LOG_INFO("[ASIO] Asio version [{}]", m_DriverInformation.asioVersion);
			LOG_INFO("[ASIO] Asio Error [{}]", m_DriverInformation.errorMessage);

			m_DeviceDetails.name = m_DriverInformation.name;

			// Get device settings
			code = ASIOGetChannels(&m_DeviceDetails.inputChannels, &m_DeviceDetails.outputChannels);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not get channels for asio device", code);

			code = ASIOGetBufferSize(&m_DeviceDetails.minBufferSize, &m_DeviceDetails.maxBufferSize, &m_DeviceDetails.prefferedBufferSize, &m_DeviceDetails.granularityBuffer);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not get buffer size for device", code);

			ASIOSampleRate sampleRate;
			code = ASIOGetSampleRate(&sampleRate);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not get sample rate for asio device", code);
			m_DeviceDetails.sampleRate = (float)sampleRate;

			code = ASIOSetSampleRate(sampleRate);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not set sample rate for asio device", code);

			return code == 0;
		}

		bool AsioAudioDevice::SetupBuffers()
		{
			m_Buffers = new ASIOBufferInfo[m_DeviceDetails.inputChannels + m_DeviceDetails.outputChannels];
			ASIOBufferInfo* bufferPtr = m_Buffers; // Create other pointer so the one pointing to the buffer doesn't increase :)

			// Prepare input buffers
			for (int i = 0; i < m_DeviceDetails.inputChannels; i++, bufferPtr++)
			{
				bufferPtr->isInput = ASIOTrue;
				bufferPtr->channelNum = i;
				bufferPtr->buffers[0] = bufferPtr->buffers[1] = 0;
			}

			// Prepare output buffers
			for (int i = 0; i < m_DeviceDetails.outputChannels; i++, bufferPtr++)
			{
				bufferPtr->isInput = ASIOFalse;
				bufferPtr->channelNum = i;
				bufferPtr->buffers[0] = bufferPtr->buffers[1] = 0;
			}

			return true;
		}

		bool AsioAudioDevice::LoadChannelInformation()
		{
			long totalChannels = m_DeviceDetails.inputChannels + m_DeviceDetails.outputChannels;
			m_Channels = new ASIOChannelInfo[totalChannels];

			for (int i = 0; i < totalChannels; i++)
			{
				m_Channels[i].channel = m_Buffers[i].channelNum;
				m_Channels[i].isInput = m_Buffers[i].isInput;
				int result = ASIOGetChannelInfo(&m_Channels[i]);
				LOG_ASSERT(result == 0, "Could not get channel information for channel {}", i);
				if (result != ASE_OK)
					return false;
			}

			return true;
		}

		static auto input_vector = std::vector<float>();

		bool AsioAudioDevice::SetupDeviceCallbacks()
		{
			s_CurrentContext = this;

			m_Callbacks.asioMessage = [](long selector, long value, void* message, double* opt) -> long {
				return s_CurrentContext->OnAsioMessage(selector, value, message, opt);
			};

			m_Callbacks.bufferSwitch = [](long doubleBufferIndex, ASIOBool directProcess) {
				s_CurrentContext->OnAsioBufferSwitch(doubleBufferIndex, directProcess);
			};

			m_Callbacks.bufferSwitchTimeInfo = [](ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess) -> ASIOTime* {
				return s_CurrentContext->OnAsioBufferSwitchTimeInfo(params, doubleBufferIndex, directProcess);
			};

			m_Callbacks.sampleRateDidChange = [](ASIOSampleRate sRate) {
				s_CurrentContext->OnAsioSampleRateDidChange(sRate);
			};

			int code = ASIOCreateBuffers(m_Buffers, m_DeviceDetails.inputChannels + m_DeviceDetails.outputChannels, m_DeviceDetails.prefferedBufferSize, &m_Callbacks);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not create buffers for device", code);

			input_vector.resize(m_DeviceDetails.prefferedBufferSize * 4);

			return code == 0;
		}

		long AsioAudioDevice::OnAsioMessage(long selector, long value, void* message, double* opt)
		{
			// TODO:: Handle asio messages

			return 0;
		}

		void AsioAudioDevice::OnAsioBufferSwitch(long doubleBufferIndex, ASIOBool& process)
		{
			ASIOTime timeInfo;

			// get the time stamp of the buffer, not necessary if no
			// synchronization to other media is required
			if (ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
				timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

			OnAsioBufferSwitchTimeInfo(&timeInfo, doubleBufferIndex, process);
		}

		ASIOTime* AsioAudioDevice::OnAsioBufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool& directProcess)
		{
			auto buffersCount = m_DeviceDetails.inputChannels + m_DeviceDetails.outputChannels;
			auto buffersize = m_DeviceDetails.prefferedBufferSize * 4;

			// TODO: support double buffers etc...

			for (size_t i = 0; i < buffersCount; i++)
			{
				if (!m_Buffers[i].isInput)
					continue;

				void* asio_input_buffer = m_Buffers[i].buffers[doubleBufferIndex];

				m_AudioConverter->ConvertToFloat(asio_input_buffer, input_vector, buffersize);

				if (!m_OnInputReady)
					return nullptr;

				m_OnInputReady(input_vector);

				break;
			}

			for (size_t i = 0; i < buffersCount; i++)
			{
				if (m_Buffers[i].isInput)
					continue;

				void* bufferOut = m_Buffers[i].buffers[doubleBufferIndex];

				m_AudioConverter->ConvertFromFloat(input_vector, bufferOut);

				break;
			}

			return nullptr;
		}

		void AsioAudioDevice::OnAsioSampleRateDidChange(ASIOSampleRate& sRate)
		{
			
		}
	}
}