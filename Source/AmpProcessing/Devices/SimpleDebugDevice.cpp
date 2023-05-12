#include "amppch.h"
#include "SimpleDebugDevice.h"

namespace AmpProcessing {

    namespace Devices {
        SimpleDebugDevice::SimpleDebugDevice() : m_SampleThread(nullptr), m_Running(false), m_DebugSample(128),
            m_DeviceDetails(DeviceDetails {"Debug device", 0, 1, 128, 1024, 128, 64, 44100.f})
        {
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
                    endTime = std::chrono::high_resolution_clock::now();
                    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                    startTime = endTime;

                    auto sleep = 10 - deltaTime.count();
                    if (sleep > 0)
                        Sleep(sleep);
                };
            });

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
            m_DebugSample.resize(bufferSize);

            return true;
        }
    }
}
