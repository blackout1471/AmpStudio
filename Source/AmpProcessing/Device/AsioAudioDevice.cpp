#include "AsioAudioDevice.h"
#include "Logging/Logger.h"

namespace AmpProcessing {
	namespace Device {

		// This is stupid, to be able to get the current context for callbacks.
		// We have to store the object which should be used for function binding.
		AsioAudioDevice* AsioAudioDevice::s_CurrentContext = nullptr;

		AsioAudioDevice::AsioAudioDevice()
		{
		}

		AsioAudioDevice::~AsioAudioDevice()
		{
		}

		bool AsioAudioDevice::Open(const std::string& deviceName)
		{
			LOG_ASSERT(deviceName != "", "Device name cannot be null");

			LOG_ASSERT(LoadDriver(deviceName), "Could not load drivername {}", deviceName);

			LOG_ASSERT(LoadDeviceDetails(), "Could not load device details");

			LOG_ASSERT(SetupBuffers(), "Could not setup buffers");

			LOG_ASSERT(SetupDeviceCallbacks(), "Could not setup callbacks for device");

			LOG_ASSERT(ASIOStart() == 0, "Could not start asio");
			return false;
		}

		bool AsioAudioDevice::Close()
		{
			return false;
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
			char* name = strdup(driverName.c_str());
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
			m_DeviceDetails.sampleRate = sampleRate;

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

		float s_CurrentSampleTime = 0;

		ASIOTime* AsioAudioDevice::OnAsioBufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool& directProcess)
		{
			auto buffersCount = m_DeviceDetails.inputChannels + m_DeviceDetails.outputChannels;
			auto inputIndexes = std::vector<int>(m_DeviceDetails.inputChannels);
			auto outputIndexes = std::vector<int>(m_DeviceDetails.outputChannels);

			// TODO: support double buffers etc...

			for (size_t i = 0; i < buffersCount; i++)
			{
				if (m_Buffers[i].isInput)
					inputIndexes.push_back(i);
				else
					outputIndexes.push_back(i);
			}

			for (size_t i = 0; i < inputIndexes.size(); i++)
			{
				for (size_t k = 0; k < outputIndexes.size(); k++)
				{
					int* outBuf = (int*)m_Buffers[outputIndexes[k]].buffers[doubleBufferIndex];

					for (size_t t = 0; t < m_DeviceDetails.prefferedBufferSize; t++)
					{
						if (s_CurrentSampleTime >= 1000000.f)
							s_CurrentSampleTime = 0;

						constexpr float double_pi = 2 * 3.14159265358979323846;
						float delta_time = 1.f / m_DeviceDetails.sampleRate;
						constexpr int frequenzy = 100;

						auto sin = std::sin(double_pi * frequenzy * s_CurrentSampleTime);

						*outBuf = static_cast<int16_t>(sin * 32768.0f);
						outBuf++;

						s_CurrentSampleTime += delta_time;
					}

					break;
				}

				break;
			}

			return nullptr;
		}

		void AsioAudioDevice::OnAsioSampleRateDidChange(ASIOSampleRate& sRate)
		{
			
		}
	}
}