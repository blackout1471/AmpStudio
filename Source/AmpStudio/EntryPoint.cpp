#include "aspch.h"
#include <Device/AsioAudioDevice.h>
#include <Logging/Logger.h>
#include <queue>

using namespace AmpProcessing::Device;

static auto bufferSamples = std::queue<std::vector<float>>();

void OnInputReady(const std::vector<float>& input) 
{
	bufferSamples.push(input);
}

std::vector<float> OnOutputReady() {
	if (bufferSamples.empty())
		return std::vector<float>();

	auto val = bufferSamples.front();
	bufferSamples.pop();
	return val;
}

int main() {
	
	AmpProcessing::Logger::Logger::Get().Init();
	auto device = AsioAudioDevice();
	const auto& names = device.GetDeviceNames();
	device.m_OnInputReady = OnInputReady;
	device.m_OnOutput = OnOutputReady;
	device.Open(names.front());

	while (true) {
		Sleep(3000);
	}

	device.Close();

	return 0;
}