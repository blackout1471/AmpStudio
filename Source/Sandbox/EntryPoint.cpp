#include "AudioFile.h"
#include <Utility/ConvolutionUtility.h>
#include <Devices/AsioAudioDevice.h>
#include <Logging/Logger.h>
#include <Utility/MESA.h>
#include <queue>

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

void DoRealTimeConvolution() {
	auto device = AsioAudioDevice();
	auto names = device.GetDeviceNames();

	auto processedQueue = std::queue<std::vector<float>>();
	auto unprocessedQueue = std::queue<std::vector<float>>();
	int* delayAmount = new int(25);
	int* delayCounter = new int(0);

	std::mutex processQueueLock;
	
	device.m_OnInputReady = [&](std::vector<float>& sample) {
		unprocessedQueue.push(sample);
		if (*delayCounter <= *delayAmount) {
			for (size_t i = 0; i < sample.size(); i++)
			{
				sample[i] = 0.f;
			}
			(*delayCounter)++;
		}
		else 
		{
			if (processedQueue.empty())
				return;

			std::lock_guard<std::mutex> lock(processQueueLock);
			auto processed = processedQueue.front();
			for (size_t i = 0; i < sample.size(); i++)
			{
				sample[i] = processed[i];
			}

			processedQueue.pop();
		}
	};

	device.Open(names.front());

	while (true) {
		while (!unprocessedQueue.empty()) {
			std::lock_guard<std::mutex> lock(processQueueLock);

			auto input = unprocessedQueue.front();
			auto result = Convolution::ConvolutionUtility::Convolution(input, frames);

			processedQueue.push(result);
			unprocessedQueue.pop();
		}
	};
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