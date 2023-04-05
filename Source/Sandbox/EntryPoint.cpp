#include "AudioFile.h"
#include <Utility/ConvolutionUtility.h>
#include <Devices/AsioAudioDevice.h>
#include <Logging/Logger.h>
#include <queue>
#include <Utility/MESA.h>

using namespace AmpProcessing;
using namespace AmpProcessing::Devices;

void OnInput(std::vector<float>& data) {

}

void DoLoopBackDemo() {
	auto device = AsioAudioDevice();
	auto names = device.GetDeviceNames();

	device.m_OnInputReady = OnInput;
	device.Open(names.front());

	while (true) {
		Sleep(5000);
	}
}


static void Log(const std::string& message) {
    std::cout << message << std::endl;
}

int main() {
	AmpProcessing::Logger::Logger::Get().Init();

	Log("1. Loopback");
	std::string input;

	std::cin >> input;

	if (input == "1")
		DoLoopBackDemo();
}