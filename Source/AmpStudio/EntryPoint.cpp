#include "aspch.h"
#include <Device/AsioAudioDevice.h>
#include <Logging/Logger.h>

using namespace AmpProcessing::Device;

int main() {
	AmpProcessing::Logger::Logger::Get().Init();

	auto device = AsioAudioDevice();
	
	const auto& names = device.GetDeviceNames();

	device.Open(names.front());

	while (true) {
		Sleep(3000);
	}

	device.Close();

	return 0;
}