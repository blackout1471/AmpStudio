#pragma once
#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"
#include <vector>
#include <string>
#include "AudioDeviceDetails.h"

namespace AmpProcessing {
	namespace Device {
		class AsioAudioDevice
		{
		public:
			AsioAudioDevice();
			~AsioAudioDevice();

			bool Open(const std::string& deviceName);
			bool Close();

		public:
			const std::vector<std::string> GetDeviceNames();

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
			AudioDeviceDetails m_DeviceDetails;
			ASIOChannelInfo* m_Channels;
			ASIOBufferInfo* m_Buffers;
			ASIOCallbacks m_Callbacks;

		private:
			static AsioAudioDevice* s_CurrentContext;
		};
	}
}