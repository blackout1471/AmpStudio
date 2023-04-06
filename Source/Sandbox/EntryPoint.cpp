#include "AudioFile.h"
#include <Utility/ConvolutionUtility.h>
#include <Devices/AsioAudioDevice.h>
#include <Logging/Logger.h>
#include <queue>
#include <Utility/MESA.h>

using namespace AmpProcessing;
using namespace AmpProcessing::Devices;

void DoLoopBackDemo() {
	auto device = AsioAudioDevice();
	auto names = device.GetDeviceNames();

	device.m_OnInputReady = [](std::vector<float>& data) {};
	device.Open(names.front());

	LOG_INFO("Playing back audio");
	while (true) {
		Sleep(5000);
	}
}

int main() {
	AmpProcessing::Logger::Logger::Get().Init();

	LOG_INFO("Enter '1' for loopback demo");
	std::string input;

	std::cin >> input;

	if (input == "1")
		DoLoopBackDemo();
}