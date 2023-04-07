#pragma once
#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"
#include "DeviceDetails.h"
#include "Converters/IAudioConverter.h"

namespace AmpProcessing {
	namespace Devices {
		class AsioAudioDevice
		{
		public:
			AsioAudioDevice();
			~AsioAudioDevice();

			bool Open(const std::string& deviceName);
			bool Close();

		public:
			const std::vector<std::string> GetDeviceNames();
			const uint32_t GetSampleRate() const { return m_DeviceDetails.sampleRate; }

		private:
			bool LoadDriver(const std::string& driverName);
			bool LoadDeviceDetails();
			bool SetupBuffers();
			bool LoadChannelInformation();
			bool SetupDeviceCallbacks();

			long OnAsioMessage(long selector, long value, void* message, double* opt);
			void OnAsioBufferSwitch(long doubleBufferIndex, ASIOBool& process);
			ASIOTime* OnAsioBufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool& directProcess);
			void OnAsioSampleRateDidChange(ASIOSampleRate& sRate);

		private:
			AsioDrivers m_AsioDriver;
			ASIODriverInfo m_DriverInformation;
			DeviceDetails m_DeviceDetails;
			ASIOCallbacks m_Callbacks;
			std::unique_ptr<ASIOChannelInfo[]> m_Channels;
			std::unique_ptr<ASIOBufferInfo[]> m_Buffers;
			std::unique_ptr<Converters::IAudioConverter> m_AudioConverter;

		public: // Todo: create function wrappers to assign
			std::function<void(std::vector<float>&)> m_OnInputReady;

		private:
			static AsioAudioDevice* s_CurrentContext;
		};
	}
}