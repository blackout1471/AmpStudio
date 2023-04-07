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

void DoRecordingDemo() {
	auto device = AsioAudioDevice();
	auto names = device.GetDeviceNames();
	auto filename = "recordingDemo.wav";

	std::vector<float> buffer;

	device.m_OnInputReady = [&](std::vector<float>& data) 
	{
		buffer.insert(buffer.end(), data.begin(), data.end());
	};
	device.Open(names.front());

	LOG_INFO("Recording 5 seconds");
	Sleep(5000);
	device.Close();

	LOG_INFO("Saving file to {}", filename);
	AudioFile<float> out;
	out.setBitDepth(24);
	out.setSampleRate(44100);
	out.setAudioBuffer(std::vector<std::vector<float>> { buffer });
	out.save(filename);
}

int main() {
	AmpProcessing::Logger::Logger::Get().Init();

	LOG_INFO("Enter '1' for loopback demo");
	LOG_INFO("Enter '2' for recording to wav file");
	std::string input;

	std::cin >> input;

	if (input == "1")
		DoLoopBackDemo();
	
	if (input == "2")
		DoRecordingDemo();
}