#include "AsioAudioDevice.h"
#include "Logging/Logger.h"

namespace AmpProcessing {
	namespace Device {

		bool AsioAudioDevice::Initialise(const std::string& deviceName)
		{
			LOG_ASSERT(deviceName == "", "Devicename cannot be null");

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
	}
}