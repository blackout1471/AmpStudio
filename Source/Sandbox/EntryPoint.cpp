#include "AudioFile.h"
#include <Utility/ConvolutionUtility.h>
#include <Devices/AsioAudioDevice.h>
#include <Logging/Logger.h>
#include <Utility/MESA.h>
#include <Utility/FFTUtility.h>
#include <queue>
#include <FFTConvolver/FFTConvolver.h>

using namespace AmpProcessing;
using namespace AmpProcessing::Devices;
using namespace AmpProcessing::Convolution;


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

void DoRealTimeConvolution() {
	auto device = AsioAudioDevice();
	auto names = device.GetDeviceNames();

	auto convolver = fftconvolver::FFTConvolver();
	auto success = convolver.init(128 * 2, &frames[0], frames.size());
	auto output = std::vector<float>(128);

	device.m_OnInputReady = [&](std::vector<float>& sample) {
		
		convolver.process(&sample[0], &output[0], 128);

		sample.assign(output.begin(), output.end());
	};
	device.Open(names.front());



	Sleep(10000);
}

int main() {
	AmpProcessing::Logger::Logger::Get().Init();

	LOG_INFO("Enter '1' for loopback demo");
	LOG_INFO("Enter '2' for recording to wav file");
	LOG_INFO("Enter '3' for realtime convolution");
	std::string input;

	std::cin >> input;

	if (input == "1")
		DoLoopBackDemo();
	
	if (input == "2")
		DoRecordingDemo();

	if (input == "3")
		DoRealTimeConvolution();
}