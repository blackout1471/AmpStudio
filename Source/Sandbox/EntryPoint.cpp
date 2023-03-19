#include "AudioFile.h"
#include "../AmpProcessing/ConvolutionUtility.h"

static std::vector<float> get_next_input_block(std::vector<float>& input, std::vector<float>::iterator& startPosition, uint32_t blocksize) {
    auto endPosition = startPosition + blocksize;

    if (endPosition >= input.end())
        return std::vector<float>(input.end() - blocksize, input.end());

    auto output = std::vector<float>(startPosition, endPosition);
    startPosition += blocksize;
    return output;
}

static void Log(const std::string& message) {
    std::cout << message << std::endl;
}

int main() {

    // Read data
    AudioFile<float> ir;
    ir.load("C:/Repos/resources/greathall.wav");

    AudioFile<float> input;
    input.load("C:/Repos/resources/example.wav");

    Log("Loaded files");

    // Initialize the filter kernel and overlap buffer
    int filter_size = 1024 * 5;
    std::vector filter_kernel(ir.samples[0].begin(), ir.samples[0].begin() + filter_size);
    std::vector<float> overlap(filter_kernel.size() - 1);

    // Process the input signal in blocks
    int block_size = 1024;
    std::vector<float> input_block(block_size);
    std::vector<float> output_block(block_size);
    std::vector<float> output_signal;

    uint32_t currentJump = 0;
    auto amount = input.samples[0].size() / block_size;

    Log("Beginning processing");

    output_block = Convolution::ConvolutionUtility::Convolution(input.samples[0], filter_kernel);


    Log("Writing to file");

    AudioFile<float> out;
    out.setSampleRate(44100);
    out.setAudioBuffer(std::vector<std::vector<float>>{output_block, output_block});
    out.save("C:/Repos/resources/overlapp.wav", AudioFileFormat::Wave);

    return 0;
}