#include "amppch.h"
#include "SimpleDebugDevice.h"

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace AmpProcessing {

    static HWAVEOUT hWaveOut = 0;
    static WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    static char buffer[8000 * 60] = {};
    static WAVEHDR header;

    namespace Devices {
        SimpleDebugDevice::SimpleDebugDevice() : m_SampleThread(nullptr), m_Running(false), m_DebugSample(128),
            m_DeviceDetails(DeviceDetails {"Debug device", 0, 1, 128, 1024, 128, 64, 44100.f})
        {
            waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

            // See http://goo.gl/hQdTi
            for (DWORD t = 0; t < sizeof(buffer); ++t)
                buffer[t] = static_cast<char>((((t * (t >> 8 | t >> 9) & 46 & t >> 8)) ^ (t & t >> 13 | t >> 6)) & 0xFF);

            header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        }

        SimpleDebugDevice::~SimpleDebugDevice()
        {
            Close();
        }

        bool SimpleDebugDevice::Open(const std::string& deviceName)
        {
            m_Running = true;
            m_SampleThread = std::make_unique<std::thread>([&]() {
                std::chrono::steady_clock::time_point endTime;
                std::chrono::milliseconds deltaTime;

                auto startTime = std::chrono::high_resolution_clock::now();
                while (m_Running) {
                    InvokeSampleReady(m_DebugSample);

                    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
                    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
                };
            });

            return true;
        }

        bool SimpleDebugDevice::Close()
        {
            m_Running = false;
            
            waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
            waveOutClose(hWaveOut);

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
            m_DebugSample.resize(bufferSize);

            return true;
        }
    }
}
