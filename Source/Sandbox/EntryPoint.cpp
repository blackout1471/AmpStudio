#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>

#pragma comment(lib, "Winmm.lib")
using namespace std;

// Define WAV file header struct
struct WavHeader {
    char riffHeader[4];
    unsigned int fileSize;
    char waveHeader[4];
    char fmtHeader[4];
    unsigned int fmtChunkSize;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char dataHeader[4];
    unsigned int dataSize;
};

std::vector<float> ReadWavFile(const std::string& path, WAVEFORMATEX& wf) {
    std::vector<float> buffer;
    
    // Open WAV file
    ifstream file(path, ios::binary | ios::in);
    if (!file) {
        cerr << "Failed to open file: " << path << endl;
        return buffer;
    }

    // Read WAV file header
    WavHeader header;
    file.read((char*)&header, sizeof(header));
    if (strncmp(header.riffHeader, "RIFF", 4) != 0 ||
        strncmp(header.waveHeader, "WAVE", 4) != 0 ||
        strncmp(header.fmtHeader, "fmt ", 4) != 0 ||
        strncmp(header.dataHeader, "data", 4) != 0) {
        cerr << "Invalid WAV file format: " << path << endl;
        return buffer;
    }

    // Read WAV file data
    const uint32_t numBytes = header.dataSize;
    const uint32_t numSamples = numBytes / sizeof(float);
    buffer.resize(numSamples);
    file.read((char*)buffer.data(), numBytes);

    // Close WAV file
    file.close();

    // Initialize audio output format
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = header.numChannels;
    wf.nSamplesPerSec = header.sampleRate;
    wf.nAvgBytesPerSec = header.byteRate;
    wf.nBlockAlign = header.blockAlign;
    wf.wBitsPerSample = header.bitsPerSample;
    wf.cbSize = 0;

    return buffer;
}

void PlayWavBuffer(const std::vector<float>& buffer, const WAVEFORMATEX& wf) {
    // Open audio output device
    HWAVEOUT hwo = NULL;
    MMRESULT result = waveOutOpen(&hwo, WAVE_MAPPER, &wf, NULL, NULL, CALLBACK_NULL);
    if (result != MMSYSERR_NOERROR) {
        cerr << "Failed to initialize audio: " << result << endl;
        return;
    }

    // Prepare audio buffer
    const uint32_t numBytes = buffer.size() * sizeof(float);
    WAVEHDR hdr;
    hdr.lpData = (LPSTR)buffer.data();
    hdr.dwBufferLength = numBytes;
    hdr.dwFlags = 0;
    hdr.dwLoops = 0;
    result = waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));
    if (result != MMSYSERR_NOERROR) {
        cerr << "Failed to prepare audio buffer: " << result << endl;
        waveOutClose(hwo);
        return;
    }

    // Play audio buffer
    result = waveOutWrite(hwo, &hdr, sizeof(hdr));
    if (result != MMSYSERR_NOERROR) {
        cerr << "Failed to play audio buffer: " << result << endl;
        waveOutUnprepareHeader(hwo, &hdr, sizeof(hdr));
        waveOutClose(hwo);
        return;
    }

    // Wait for audio to finish playing
    while ((hdr.dwFlags & WHDR_DONE) != WHDR_DONE) {
        Sleep(10);
    }

    // Unprepare audio buffer
    result = waveOutUnprepareHeader(hwo, &hdr, sizeof(hdr));
    if (result != MMSYSERR_NOERROR) {
        cerr << "Failed to unprepare audio buffer: " << result << endl;
    }

    // Close audio output device
    waveOutClose(hwo);
}

int main() {

	const auto recording = "C:/Repos/resources/Recording.wav";
	const auto testRec = "C:/Repos/resources/test.wav";

	std::cout << "Hello Sandbox!" << std::endl;

    WAVEFORMATEX wf;
    auto buffer = ReadWavFile(testRec, wf);
    PlayWavBuffer(buffer, wf);

	// Set the overlap-add block size and hop size
	const int blockSize = 1024;
	const int hopSize = 512;
	const float mixRatio = 0.7;

	// Initialize the overlap-add buffer
	//std::vector<float> overlapBuffer(blockSize + filter.Size() - 1, 0);

	//// Convolve the sound buffer with the filter
	//for (int i = 0; i < buffer.Size(); i += hopSize) {
	//	// Copy the next block of sound data to the input buffer
	//	std::vector<float> inputBuffer(blockSize);
	//	buffer.Copy(inputBuffer, i, blockSize);

	//	// Convolve the input buffer with the filter
	//	std::vector<float> outputBuffer(blockSize + filter.Size() - 1);
	//	Convolve(inputBuffer, filter, outputBuffer);

	//	// Add the convolved block to the overlap-add buffer
	//	for (int j = 0; j < blockSize; j++) {
	//		overlapBuffer[i + j] += outputBuffer[j];
	//	}
	//}

	//// Normalize the overlap-add buffer
	//float maxSample = *std::max_element(overlapBuffer.begin(), overlapBuffer.end());
	//for (int i = 0; i < overlapBuffer.size(); i++) {
	//	overlapBuffer[i] /= maxSample;
	//}

	//// Mix the convolved sound with the original sound buffer
	//for (int i = 0; i < buffer.Size(); i++) {
	//	buffer[i] = (1 - mixRatio) * buffer[i] + mixRatio * overlapBuffer[i];
	//}

	return 0;
}