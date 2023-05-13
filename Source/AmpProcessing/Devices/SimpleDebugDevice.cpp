#include "amppch.h"
#include "SimpleDebugDevice.h"
#include <condition_variable>
#include <atomic>

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#pragma comment(lib, "winmm.lib")

#include "Utility/AudioFile.h"

namespace AmpProcessing {

    namespace Devices {
        SimpleDebugDevice::SimpleDebugDevice() : m_SampleThread(), m_Running(false), m_InputData(), m_SplitData(),
            m_DeviceDetails(DeviceDetails {"Debug device", 0, 1, 128, 1024, 128, 64, 44100.f}), m_Available(), m_Device(),
            m_MuxBlock(), m_BlockLock()
        {
        }

        SimpleDebugDevice::~SimpleDebugDevice()
        {
            Close();
        }

        void CALLBACK SimpleDebugDevice::waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
        {
            reinterpret_cast<SimpleDebugDevice*>(dwInstance)->WaveOutProc(hWaveOut, uMsg, dwParam1, dwParam2);
        }

        bool SimpleDebugDevice::Open(const std::string& deviceName)
        {
            AudioFile<float> audioFile;
            audioFile.load("C:\\Repos\\resources\\guitar-dry.wav");
            m_InputData = audioFile.samples[0];
            SetBufferSize(m_DeviceDetails.prefferedBufferSize);
            m_Device = CreateDevice(audioFile.getSampleRate());

            // Play audio in loop
            m_Running = true;
            m_SampleThread = std::thread(&SimpleDebugDevice::DeviceThread, this);

            return true;
        }

        bool SimpleDebugDevice::Close()
        {
            m_Running = false;

            return true;
        }

        const std::vector<std::string> SimpleDebugDevice::GetDeviceNames()
        {
            return std::vector<std::string> { "DebugFakeDevice" };
        }

        const DeviceDetails& SimpleDebugDevice::GetDetails() const
        {
            return m_DeviceDetails;
        }

        bool SimpleDebugDevice::SetSampleRate(uint32_t sampleRate)
        {
            m_DeviceDetails.sampleRate = sampleRate;

            return true;
        }

        bool SimpleDebugDevice::SetBufferSize(uint32_t bufferSize)
        {
            m_SplitData = SplitDataToSize(bufferSize, m_InputData);
            return true;
        }

        HWAVEOUT SimpleDebugDevice::CreateDevice(uint32_t sampleRate)
        {
            WAVEFORMATEX wfx;
            wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT; // simple, uncompressed format
            wfx.nChannels = 1; // 1=mono, 2=stereo
            wfx.nSamplesPerSec = sampleRate;
            wfx.wBitsPerSample = 32; // 16 for high quality, 8 for telephone-grade
            wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
            wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec) * (wfx.nChannels) * (wfx.wBitsPerSample) / 8;
            wfx.cbSize = 0;


            HWAVEOUT hWaveOut = 0;
            int result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProcWrap, (DWORD_PTR)this, CALLBACK_FUNCTION);
            LOG_ASSERT(hWaveOut, "Could not create output device WIN ERROR {}", result);

            return hWaveOut;
        }

        void SimpleDebugDevice::WaveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2)
        {
            if (uMsg != WOM_DONE) return;

            m_Available.fetch_add(1);
            std::unique_lock<std::mutex> lm(m_MuxBlock);
            m_BlockLock.notify_one();
        }

        void SimpleDebugDevice::DeviceThread()
        {
            int bufferSize = m_DeviceDetails.prefferedBufferSize;
            int currentSplit = 0;
            int currentBlock = 0;
            int queueAmount = 8;
            m_Available.store(queueAmount);

            WAVEHDR* waveHeaders = new WAVEHDR[queueAmount];
            ZeroMemory(waveHeaders, sizeof(WAVEHDR) * queueAmount);
            for (size_t i = 0; i < queueAmount; i++)
            {
                waveHeaders[i].dwBufferLength = bufferSize * sizeof(float);
            }

            while (m_Running) {
                if (m_Available == 0)
                {
                    // Wait
                    std::unique_lock<std::mutex> lm(m_MuxBlock);
                    m_BlockLock.wait(lm);
                }

                m_Available.fetch_sub(1);

                if (currentSplit >= m_SplitData.size())
                    currentSplit = 0;

                if (currentBlock >= queueAmount)
                    currentBlock = 0;


                waveHeaders[currentBlock].lpData = (LPSTR)&m_SplitData[currentSplit].front();

                // Invoke sample action
                InvokeSampleReady(m_SplitData[currentSplit]);
                currentSplit++;

                // Output
                waveOutPrepareHeader(m_Device, &waveHeaders[currentBlock], sizeof(WAVEHDR));
                waveOutWrite(m_Device, &waveHeaders[currentBlock], sizeof(WAVEHDR));
                currentBlock++;
            };
        }

        std::vector<std::vector<float>> SimpleDebugDevice::SplitDataToSize(int32_t size, const std::vector<float>& data)
        {
            std::vector<std::vector<float>> samples;
            for (size_t i = 0; i < data.size(); i += size) {
                if (i + size > data.size())
                    continue;
                auto start = data.begin() + i;
                auto end = start + size;
                samples.push_back(std::vector<float>(start, end));
            }

            return samples;
        }
    }
}
