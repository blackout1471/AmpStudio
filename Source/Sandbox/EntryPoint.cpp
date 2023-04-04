#include "AudioFile.h"
#include <Utility/ConvolutionUtility.h>
#include <Device/AsioAudioDevice.h>
#include <Logging/Logger.h>
#include <queue>
#include <Utility/MESA.h>

using namespace AmpProcessing;

static void Log(const std::string& message) {
    std::cout << message << std::endl;
}

//int main() {
//
//    AudioFile<float> input;
//    input.load("C:/Repos/resources/guitar_raw.wav");
//
//    Log("Loaded files");
//
//    // Initialize the filter kernel and overlap buffer
//    int filter_size = 1024 * 5;
//    std::vector filter_kernel = frames;
//
//    Log("Beginning processing");
//
//    auto output_block = Convolution::ConvolutionUtility::OverlapAddConvolution(input.samples[0], filter_kernel, 1024);
//
//
//    Log("Writing to file");
//
//    AudioFile<float> out;
//    out.setSampleRate(44100);
//    out.setAudioBuffer(std::vector<std::vector<float>>{output_block, output_block});
//    out.save("C:/Repos/resources/guitar_mesa_overlap.wav", AudioFileFormat::Wave);
//
//    return 0;
//}

using namespace AmpProcessing::Device;
using namespace AmpProcessing::Convolution;

static auto bufferSamples = std::queue<std::vector<float>>();
static auto inputBufferSamples = std::queue<std::vector<float>>();

void OnInputReady(std::vector<float>& input) 
{
	
}

int main() {
	
	AmpProcessing::Logger::Logger::Get().Init();
	auto device = AsioAudioDevice();
	const auto& names = device.GetDeviceNames();
	device.m_OnInputReady = OnInputReady;
	device.Open(names.front());

	while (true) {
		Sleep(3000);
	}

	device.Close();

	return 0;
}