#pragma once
#include "IAudioDevice.h"

namespace AmpProcessing {
	namespace Devices {
		class SimpleDebugDevice : public IAudioDevice
		{
		public:
			SimpleDebugDevice();
			~SimpleDebugDevice();

		public:
			virtual bool Open(const std::string& deviceName) override;
			virtual bool Close() override;
			virtual const std::vector<std::string> GetDeviceNames() override;
			
			virtual const DeviceDetails& GetDetails() const override;

			virtual bool SetSampleRate(uint32_t sampleRate) override;
			virtual bool SetBufferSize(uint32_t bufferSize) override;
		private:
			std::unique_ptr<std::thread> m_SampleThread;
			std::vector<float> m_DebugSample;
			bool m_Running;
			DeviceDetails m_DeviceDetails;
		};
	}
}