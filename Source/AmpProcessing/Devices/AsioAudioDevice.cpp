#include <amppch.h>
#include "AsioAudioDevice.h"
#include "Logging/Logger.h"
#include "Utility/MESA.h"
#include "Utility/ConvolutionUtility.h"
#include "Converters/Int32LsbAudioConverter.h"

namespace AmpProcessing {
	namespace Devices {

		// This is stupid, to be able to get the current context for callbacks.
		// We have to store the object which should be used for function binding.
		// It would be better to have a mapping of instances if we in the future will need more devices.
		AsioAudioDevice* AsioAudioDevice::s_CurrentContext = nullptr;

		AsioAudioDevice::AsioAudioDevice() : m_Buffers(), m_Callbacks(), m_Channels(), m_DriverInformation(), 
			m_AudioConverter(), m_HasBeenStopped(true), m_SampleBuffer()
		{
			// TODO: Determine at runtime
			m_AudioConverter = std::make_unique<Converters::Int32LsbAudioConverter>();
		}

		AsioAudioDevice::~AsioAudioDevice()
		{
			Close();
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

			m_HasBeenStopped = false;

			return true;
		}

		bool AsioAudioDevice::Close()
		{
			if (m_HasBeenStopped)
				return true;

			LOG_ASSERT(ASIOStop() == 0, "Could not stop ASIO");

			LOG_ASSERT(ASIODisposeBuffers() == 0, "Could not dispose of asio buffers");

			m_HasBeenStopped = true;

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

			SetSampleRate(sampleRate);

			return code == 0;
		}

		bool AsioAudioDevice::SetupBuffers()
		{
			m_Buffers = std::make_unique<ASIOBufferInfo[]>(m_DeviceDetails.inputChannels + m_DeviceDetails.outputChannels);
			ASIOBufferInfo* bufferPtr = m_Buffers.get(); // Create other pointer so the one pointing to the buffer doesn't increase :)

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
			m_Channels = std::make_unique<ASIOChannelInfo[]>(totalChannels);

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

			return CreateBuffers(m_DeviceDetails.prefferedBufferSize);
		}

		bool AsioAudioDevice::CreateBuffers(uint32_t bufferSize) {

			if (!m_HasBeenStopped)
				ASIODisposeBuffers();

			int code = ASIOCreateBuffers(m_Buffers.get(), m_DeviceDetails.inputChannels + m_DeviceDetails.outputChannels, bufferSize, &m_Callbacks);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not create buffers for device", code);
			LOG_INFO("[ASIO] Buffers created with size: {}", bufferSize);

			m_SampleBuffer.resize(bufferSize);
			
			return true;
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
			auto buffersize = m_SampleBuffer.size();

			// TODO: support double buffers etc...

			for (size_t i = 0; i < buffersCount; i++)
			{
				if (!m_Buffers[i].isInput)
					continue;

				void* asio_input_buffer = m_Buffers[i].buffers[doubleBufferIndex];

				m_AudioConverter->ConvertToFloat(asio_input_buffer, m_SampleBuffer, buffersize);

				InvokeSampleReady(m_SampleBuffer);

				break;
			}

			for (size_t i = 0; i < buffersCount; i++)
			{
				if (m_Buffers[i].isInput)
					continue;

				void* bufferOut = m_Buffers[i].buffers[doubleBufferIndex];

				m_AudioConverter->ConvertFromFloat(m_SampleBuffer, bufferOut);
			}

			ASIOOutputReady();

			return nullptr;
		}

		void AsioAudioDevice::OnAsioSampleRateDidChange(ASIOSampleRate& sRate)
		{
			
		}

		bool AsioAudioDevice::SetSampleRate(uint32_t sampleRate)
		{
			int code = ASIOSetSampleRate(sampleRate);
			LOG_ASSERT(code == 0, "[ASIO] ({}): Could not set sample rate for device", code);
			m_DeviceDetails.sampleRate = sampleRate;
			LOG_INFO("[ASIO] Sample rate was changed to: {}", sampleRate);

			return true;
		}

		bool AsioAudioDevice::SetBufferSize(uint32_t bufferSize)
		{
			if (!CreateBuffers(bufferSize))
				return false;

			LOG_ASSERT(ASIOStart() == 0, "Could not start asio");
			
			return true;
		}
	}
}