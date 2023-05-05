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

		private:
			std::unique_ptr<std::thread> m_SampleThread;
			std::vector<float> m_DebugSample;
			bool m_Running;
		};
	}
}