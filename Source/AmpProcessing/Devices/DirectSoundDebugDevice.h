#pragma once
#include "IAudioDevice.h"
#include "Logging/Logger.h"
#include <mmreg.h>
#include <xaudio2.h>

#include <Utility/AudioFile.h>


namespace AmpProcessing {
	namespace Devices {
		class DirectSoundDebugDevice : public IAudioDevice {
		private:
			struct XAudioBuffer {
				XAUDIO2_BUFFER Header;
				std::vector<float> data;
			};

		public:
			inline DirectSoundDebugDevice() : m_RawAudioData(), m_SampleCounter(0), m_DeviceDetails({ "DirectSound", 1, 2, 128, 1024, 128, 1, 44100 }),
				m_Playing(false), m_CurrentBufferSize()
			{
				AudioFile<float> audioFile;
				audioFile.load("C:\\Repos\\resources\\guitar-dry.wav");
				m_RawAudioData = audioFile.samples[0];
			};
			inline ~DirectSoundDebugDevice() 
			{
				Close();
				m_SampleThread.join();
			};

			virtual bool Open(const std::string& deviceName) override
			{
				m_SampleThread = std::thread([&]() {
					HRESULT hr;

					CreateDevice();

					WAVEFORMATEX wfx;
					wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT; // simple, uncompressed format
					wfx.nChannels = 1; // 1=mono, 2=stereo
					wfx.nSamplesPerSec = m_DeviceDetails.sampleRate;
					wfx.wBitsPerSample = 32; // 16 for high quality, 8 for telephone-grade
					wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
					wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec) * (wfx.nChannels) * (wfx.wBitsPerSample) / 8;
					wfx.cbSize = 0;

					m_CurrentBufferSize = m_DeviceDetails.prefferedBufferSize;

					hr = m_XAudio2->CreateSourceVoice(&m_SourceVoice, &wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
					LOG_ASSERT(!FAILED(hr), "Could not create source voice");

					CreateNewXAudioBuffers(5, m_CurrentBufferSize);

					hr = m_SourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
					LOG_ASSERT(!FAILED(hr), "Could not start voice");
					m_Playing = true;

					int currentBuffer = 0;
					while (m_Playing) {
						XAUDIO2_VOICE_STATE state;
						m_SourceVoice->GetState(&state);

						if (state.BuffersQueued < m_XAudioBuffers.size())
						{
							auto sample = GetNextSample();
							InvokeSampleReady(sample);

							auto& currentXAOU = m_XAudioBuffers[currentBuffer];

							currentXAOU.data = sample;
							currentXAOU.Header.pAudioData = (BYTE*)&currentXAOU.data.front();
						
							hr = m_SourceVoice->SubmitSourceBuffer(&currentXAOU.Header);
							LOG_ASSERT(!FAILED(hr), "Could not submit buffer");

							currentBuffer = (currentBuffer + 1) % m_XAudioBuffers.size();
						}
					}

					CoUninitialize();
				});


				return true;
			};

			virtual bool Close() override 
			{ 
				m_Playing = false;
				m_SourceVoice->Stop(0);
				m_SourceVoice->FlushSourceBuffers();
				m_SourceVoice->DestroyVoice();
				m_MasterVoice->DestroyVoice();
				m_XAudio2->Release();
				return true;
			};

			virtual const DeviceDetails& GetDetails() const override { return m_DeviceDetails; };
			virtual bool SetSampleRate(uint32_t sampleRate) override 
			{ 
				// TODO: Be able to set new sample rate.
				return true; 
			};
			virtual bool SetBufferSize(uint32_t bufferSize) override 
			{ 
				m_CurrentBufferSize = bufferSize;
				CreateNewXAudioBuffers(5, m_CurrentBufferSize);
				return true; 
			};
			virtual const std::vector<std::string> GetDeviceNames() override { return { "DirectSoundDebug" }; };

		private:
			inline void CreateNewXAudioBuffers(int amount, uint32_t bufferSize) {
				m_XAudioBuffers.clear();

				for (size_t i = 0; i < amount; i++)
				{
					XAUDIO2_BUFFER xaudio = { 0 };
					xaudio.AudioBytes = bufferSize * sizeof(float);
					xaudio.Flags = XAUDIO2_END_OF_STREAM;
					m_XAudioBuffers.push_back({ xaudio, std::vector<float>()});
				}
			}
			inline std::vector<float> GetNextSample() {
				auto bufferSize = m_CurrentBufferSize;
				if (m_SampleCounter + bufferSize > m_RawAudioData.size())
					m_SampleCounter = 0;

				auto start = m_RawAudioData.begin() + m_SampleCounter;
				auto sample = std::vector<float>(start, start + bufferSize);
				m_SampleCounter += bufferSize;
				return sample;
			}
			inline void CreateDevice() {
				HRESULT hr;

				hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
				LOG_ASSERT(!FAILED(hr), "Could not co initialise");

				hr = XAudio2Create(&m_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
				LOG_ASSERT(!FAILED(hr), "Could not create direct sound device");

				hr = m_XAudio2->CreateMasteringVoice(&m_MasterVoice);
				LOG_ASSERT(!FAILED(hr), "Could not create mastering voice");
			}

		private:
			bool m_Playing;
			std::vector<float> m_RawAudioData;
			int m_SampleCounter;
			long m_CurrentBufferSize;

			std::vector<XAudioBuffer> m_XAudioBuffers;
			std::thread m_SampleThread;

			DeviceDetails m_DeviceDetails;

			IXAudio2* m_XAudio2;
			IXAudio2MasteringVoice* m_MasterVoice;
			IXAudio2SourceVoice* m_SourceVoice;


		};
	}
}