#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "AudioFile.h"
#include <thread>


std::vector<float> convolution(const std::vector<float>& buffer, const std::vector<float>& ir) {
    int buffer_size = buffer.size();
    int ir_size = ir.size();
    std::vector<float> output(buffer_size + ir_size - 1);
    int output_size = output.size();

    // Create temporary buffer for convolution
    std::vector<float> temp_buffer(output_size, 0);

    // Copy buffer to temp buffer
    std::copy(buffer.begin(), buffer.end(), temp_buffer.begin());

    // Convolve buffer with impulse response
    for (int i = 0; i < output_size; i++) {
        output[i] = 0;
        for (int j = 0; j < ir_size; j++) {
            if (i >= j && i - j < buffer_size) {
                output[i] += temp_buffer[i - j] * ir[j];
            }
        }
    }

    return output;
}

std::vector<float> delay_filter(int delay_samples, float feedback_gain) {
    // Create impulse response buffer
    std::vector<float> ir(delay_samples + 1, 0);

    // Set delay sample to 1
    ir[delay_samples] = 1.0f;

    // Set feedback sample to gain
    ir[0] = feedback_gain;

    return ir;
}

std::vector<float> generate_reverb_ir(float decay_time, int sample_rate, int length) {
    std::vector<float> ir(length);
    const float decay_factor = std::pow(10.0f, -3.0f * length / (decay_time * sample_rate));
    ir[0] = 1.0f;
    for (int i = 1; i < length; i++) {
        ir[i] = decay_factor * ir[i - 1];
    }
    return ir;
}

#define M_PI 3.14159

bool convolution_unit() {
    // Set up input buffer and impulse response
    const std::vector<float> buffer = { 1, 2, 3, 4 };
    const std::vector<float> ir = { 0.5, 1, 0.5 };

    // Expected output
    const std::vector<float> expected_output = { 0.5, 2, 4, 6, 5.5, 2 };

    // Perform convolution
    auto output_buffer = convolution(buffer, ir);

    // Check that the output matches the expected result
    if (output_buffer.size() != expected_output.size())
        return false;

    for (int i = 0; i < output_buffer.size(); i++) {
        if (output_buffer[i] != expected_output[i])
            return false;
    }

    return true;
}

int main() {
    // check simple unit
    if (!convolution_unit())
        std::cout << "Failed unit" << std::endl;

    AudioFile<float> file;
    file.load("C:/Repos/resources/example.wav");

    AudioFile<float> input_file;
    input_file.load("C:/Repos/resources/greathall.wav");

    // Set up input buffer with a sine wave
    auto input_buffer = file.samples[0];
    auto ir_buffer = input_file.samples[0];
    //int buffer_size = input_buffer.size();

    //// Create delay filter with 1 second delay and 0.5 feedback gain
    //const int delay_samples = file.getSampleRate() * 1;
    //const float feedback_gain = 0.7f;
    //std::vector<float> ir_filter = delay_filter(delay_samples, feedback_gain);

    //// Create reverb
    //float decay_time = 5.0f;    // decay time in seconds
    //int sample_rate = 44100;   // sample rate in Hz
    //int ir_length = sample_rate * 2; // length of the impulse response
    //std::vector<float> ir = generate_reverb_ir(decay_time, sample_rate, ir_length);

    //// Normalize the impulse response
    //float max_ir = *std::max_element(ir.begin(), ir.end(), [](float a, float b) { return std::abs(a) < std::abs(b); });
    //for (auto& sample : ir) {
    //    sample /= max_ir;
    //}

    std::thread irThread([&]() {
        auto delay_output = convolution(input_buffer, ir_buffer);
        AudioFile<float> delay;
        delay.setAudioBuffer(std::vector<std::vector<float>> { delay_output, delay_output });
        delay.setSampleRate(44100);
        delay.save("C:/Repos/resources/delay.wav", AudioFileFormat::Wave);
    });

    /*std::thread reverbThread([&]() {
        auto reverb_output = convolution(input_buffer, ir);
        AudioFile<float> reverb;
        reverb.setAudioBuffer(std::vector<std::vector<float>> { reverb_output, reverb_output });
        reverb.setSampleRate(44100);
        reverb.save("C:/Repos/resources/reverb.wav", AudioFileFormat::Wave);
    });*/

    irThread.join();
    //reverbThread.join();

    return 0;
}

//int main() {
//
//	const auto recording = "C:/Repos/resources/Recording.wav";
//	const auto testRec = "C:/Repos/resources/test.wav";
//
//	std::cout << "Hello Sandbox!" << std::endl;
//
//    WAVEFORMATEX wf;
//    auto rec_buffer = ReadWavFile(testRec, wf);
//    auto ir_buffer = delay_filter(5, 0.8);
//
//    int inputSize = rec_buffer.size();
//    int irSize = ir_buffer.size();
//    int outputSize = inputSize + irSize - 1;
//
//    // pad
//    ir_buffer.resize(outputSize, 0);
//
//    auto output_buffer = std::vector<float>(outputSize);
//
//    convolution(rec_buffer, ir_buffer, output_buffer);
//
//    PlayWavBuffer(rec_buffer, wf);
//    PlayWavBuffer(output_buffer, wf);
//
//
//    //PlayWavBuffer(output_buffer, wf);
//
//	// Set the overlap-add block size and hop size
//	/*const int blockSize = 1024;
//	const int hopSize = 512;
//	const float mixRatio = 0.7;*/
//
//	// Initialize the overlap-add buffer
//	//std::vector<float> overlapBuffer(blockSize + filter.Size() - 1, 0);
//
//	//// Convolve the sound buffer with the filter
//	//for (int i = 0; i < buffer.Size(); i += hopSize) {
//	//	// Copy the next block of sound data to the input buffer
//	//	std::vector<float> inputBuffer(blockSize);
//	//	buffer.Copy(inputBuffer, i, blockSize);
//
//	//	// Convolve the input buffer with the filter
//	//	std::vector<float> outputBuffer(blockSize + filter.Size() - 1);
//	//	Convolve(inputBuffer, filter, outputBuffer);
//
//	//	// Add the convolved block to the overlap-add buffer
//	//	for (int j = 0; j < blockSize; j++) {
//	//		overlapBuffer[i + j] += outputBuffer[j];
//	//	}
//	//}
//
//	//// Normalize the overlap-add buffer
//	//float maxSample = *std::max_element(overlapBuffer.begin(), overlapBuffer.end());
//	//for (int i = 0; i < overlapBuffer.size(); i++) {
//	//	overlapBuffer[i] /= maxSample;
//	//}
//
//	//// Mix the convolved sound with the original sound buffer
//	//for (int i = 0; i < buffer.Size(); i++) {
//	//	buffer[i] = (1 - mixRatio) * buffer[i] + mixRatio * overlapBuffer[i];
//	//}
//
//	//return 0;
//}