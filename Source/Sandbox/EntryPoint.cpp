#include "AudioFile.h"
#include <Utility/ConvolutionUtility.h>
#include <Devices/AsioAudioDevice.h>
#include <Logging/Logger.h>
#include <Utility/MESA.h>
#include <Utility/FFTUtility.h>
#include <queue>
#include <Utility/FilterUtility.h>
#include <DSP/Convolution/FFTSampleConvolver.h>

#include <dsound.h>

using namespace AmpProcessing;
using namespace AmpProcessing::Devices;
using namespace AmpProcessing::Convolution;
using namespace AmpProcessing::Filter;
using namespace AmpProcessing::DSP;

const float M_PI = 3.14159265;

void apply_distortion(std::vector<float>& samples, float drive, float range, float blend, float volume) {
	for (size_t i = 0; i < samples.size(); i++)
	{
		float cleanValue = samples[i];

		samples[i] *= drive * range;

		samples[i] = (((2.f / M_PI) * atan(samples[i]) * blend) + (cleanValue * (1.f - blend)) / 2.f) * volume;
	}
}

void DoLoopBackDemo() {
	auto device = AsioAudioDevice();
	auto names = device.GetDeviceNames();

	device.SetSampleReadyCallback([](std::vector<float>& data) {});
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

	device.SetSampleReadyCallback([&](std::vector<float>& data) 
	{
		buffer.insert(buffer.end(), data.begin(), data.end());
	});
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

	AudioFile<float> in;
	in.load("C:\\Repos\\resources\\greathall.wav");

	auto device = AsioAudioDevice();
	auto names = device.GetDeviceNames();

	auto internal_size = int(std::pow(2.0, 10));

	auto cabConvoler = FFTSampleConvolver();
	auto success = cabConvoler.Init(internal_size, in.samples[0]);

	auto output = std::vector<float>(128);
	auto output2 = std::vector<float>(128);

	const float level = 0.2f;

	device.SetSampleReadyCallback([&](std::vector<float>& samples) {

		auto& currentIn = samples;
		
		cabConvoler.Process(currentIn, output2);
		currentIn = output2;

		//apply_distortion(currentIn, 1.f, 100.f, 1.f, 1.f);
		/*for (size_t i = 0; i < currentIn.size(); i++)
		{
			currentIn[i] *= level;
		}*/

		samples.assign(currentIn.begin(), currentIn.end());
	});
	device.Open(names.front());


	while (true) {
		Sleep(10000);
	}
}

void DoDebugDeviceDemo() {
	
}

int main() {
	AmpProcessing::Logger::Logger::Get().Init();

	LOG_INFO("Enter '1' for loopback demo");
	LOG_INFO("Enter '2' for recording to wav file");
	LOG_INFO("Enter '3' for realtime convolution");
	LOG_INFO("Enter '4' for debug device");
	std::string input;

	std::cin >> input;

	if (input == "1")
		DoLoopBackDemo();
	
	if (input == "2")
		DoRecordingDemo();

	if (input == "3")
		DoRealTimeConvolution();

	if (input == "4")
		DoDebugDeviceDemo();
}