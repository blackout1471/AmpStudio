#pragma once
#include <vector>
#include <string>

namespace AmpProcessing {
	namespace Devices {
		class IAudioDevice {
		public:
			IAudioDevice() = default;
			~IAudioDevice() = default;

			virtual bool Open(const std::string& deviceName) = 0;
			virtual bool Close() = 0;
			virtual const std::vector<std::string> GetDeviceNames() = 0;

			inline void SetSampleReadyCallback(const std::function<void(std::vector<float>&)>& callback) {
				m_OnSampleReady = callback;
			};

		protected:
			inline void InvokeSampleReady(std::vector<float>& samples) {
				if (!m_OnSampleReady)
					return;

				m_OnSampleReady(samples);
			}

		private:
			std::function<void(std::vector<float>&)> m_OnSampleReady;
		};
	}
}