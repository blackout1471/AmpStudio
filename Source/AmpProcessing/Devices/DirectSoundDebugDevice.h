#pragma once
#include "IAudioDevice.h"
#include "Logging/Logger.h"
#include <mmreg.h>
#include <xaudio2.h>

#include <Utility/AudioFile.h>


namespace AmpProcessing {
	namespace Devices {
		class DirectSoundDebugDevice : public IAudioDevice {
		public:
			inline DirectSoundDebugDevice() : m_RawAudioData(), m_SampleCounter(0), m_DeviceDetails({ "DirectSound", 1, 2, 128, 1024, 256, 1, 44100 })
			{
				AudioFile<float> audioFile;
				audioFile.load("C:\\Repos\\resources\\guitar-dry.wav");
				m_RawAudioData = audioFile.samples[0];
			};
			inline ~DirectSoundDebugDevice() {};

			virtual bool Open(const std::string& deviceName) override
			{

				IXAudio2* pXAudio2 = NULL;
				IXAudio2MasteringVoice* pMasterVoice = NULL;
				IXAudio2SourceVoice* pSourceVoice = NULL;

				HRESULT hr;

				hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
				LOG_ASSERT(!FAILED(hr), "Could not co initialise");

				// Create the DirectSound object
				hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
				LOG_ASSERT(!FAILED(hr), "Could not create direct sound device");

				// Set the cooperative level
				hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);
				LOG_ASSERT(!FAILED(hr), "Could not create mastering voice");

				// Create format
				WAVEFORMATEX wfx;
				wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT; // simple, uncompressed format
				wfx.nChannels = 1; // 1=mono, 2=stereo
				wfx.nSamplesPerSec = 44100;
				wfx.wBitsPerSample = 32; // 16 for high quality, 8 for telephone-grade
				wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
				wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec) * (wfx.nChannels) * (wfx.wBitsPerSample) / 8;
				wfx.cbSize = 0;

				// Create the source voice
				hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
				LOG_ASSERT(!FAILED(hr), "Could not create source voice");

				int internalBufferCount = 4;
				std::vector<XAUDIO2_BUFFER> buffers;
				for (size_t i = 0; i < internalBufferCount; i++)
				{
					XAUDIO2_BUFFER xaudio = { 0 };
					xaudio.AudioBytes = m_DeviceDetails.prefferedBufferSize * sizeof(float);
					xaudio.Flags = XAUDIO2_END_OF_STREAM;
					buffers.push_back(xaudio);
				}

				hr = pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
				LOG_ASSERT(!FAILED(hr), "Could not start voice");

				int currentBuffer = 0;
				std::vector<std::vector<float>> samples;
				while (true) {
					XAUDIO2_VOICE_STATE state;
					pSourceVoice->GetState(&state);

					if (state.BuffersQueued < internalBufferCount)
					{
						auto sample = GetNextSample();
						samples.push_back(sample);
						buffers[currentBuffer].pAudioData = (BYTE*)&samples.back().front();
						
						hr = pSourceVoice->SubmitSourceBuffer(&buffers[currentBuffer]);
						LOG_ASSERT(!FAILED(hr), "Could not submit buffer");

						currentBuffer = (currentBuffer + 1) % internalBufferCount;
					}
				}

				return true;
			};
			virtual bool Close() override { return true; };
			virtual const DeviceDetails& GetDetails() const override { return m_DeviceDetails; };
			virtual bool SetSampleRate(uint32_t sampleRate) override { return true; };
			virtual bool SetBufferSize(uint32_t bufferSize) override { return true; };
			
			virtual const std::vector<std::string> GetDeviceNames() override { return { "DirectSoundDebug" }; };

		private:
			std::vector<float> GetNextSample() {
				auto bufferSize = m_DeviceDetails.prefferedBufferSize;
				if (m_SampleCounter + bufferSize > m_RawAudioData.size())
					m_SampleCounter = 0;

				auto start = m_RawAudioData.begin() + m_SampleCounter;
				auto sample = std::vector<float>(start, start + bufferSize);
				m_SampleCounter += bufferSize;
				return sample;
			}

		private:
			std::vector<float> m_RawAudioData;
			int m_SampleCounter;

			DeviceDetails m_DeviceDetails;
		};
	}
}