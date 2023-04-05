#pragma once
#include <string>
namespace AmpProcessing {
	namespace Devices {
		struct AudioDeviceDetails {
			std::string name;

			long inputChannels;
			long outputChannels;

			long minBufferSize;
			long maxBufferSize;
			long prefferedBufferSize;
			long granularityBuffer;

			float sampleRate;
		};
	}
}