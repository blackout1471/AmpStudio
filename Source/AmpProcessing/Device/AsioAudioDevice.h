#pragma once
#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"
#include <vector>
#include <string>

namespace AmpProcessing {
	namespace Device {
		class AsioAudioDevice
		{
		public:
			AsioAudioDevice();
			~AsioAudioDevice();

			bool Initialise(const std::string& deviceName);

		public:
			const std::vector<std::string> GetDeviceNames();

		private:
			AsioDrivers m_AsioDriver;
		};
	}
}