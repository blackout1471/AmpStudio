#pragma once
#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"
#include "DeviceDetails.h"
#include "Converters/IAudioConverter.h"
#include "IAudioDevice.h"

namespace AmpProcessing {
	namespace Devices {
		/// <summary>
		/// Class have functionlity for how to load in an asio audio driver device.
		/// </summary>
		class AsioAudioDevice : public IAudioDevice
		{
		public:
			AsioAudioDevice();
			~AsioAudioDevice();

			bool Open(const std::string& deviceName) override;
			bool Close() override;

			inline virtual const DeviceDetails& GetDetails() const override { return m_DeviceDetails; };

			const std::vector<std::string> GetDeviceNames() override;

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
			bool m_HasBeenStopped;

		private:
			static AsioAudioDevice* s_CurrentContext;

		};
	}
}