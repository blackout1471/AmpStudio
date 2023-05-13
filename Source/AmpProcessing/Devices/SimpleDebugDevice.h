#pragma once
#include "IAudioDevice.h"
#include <mmsystem.h>

namespace AmpProcessing {
	namespace Devices {
		class SimpleDebugDevice : public IAudioDevice
		{
		public:
			SimpleDebugDevice();
			~SimpleDebugDevice();

		public:
			virtual bool Open(const std::string& deviceName) override;
			virtual bool Close() override;
			virtual const std::vector<std::string> GetDeviceNames() override;
			
			virtual const DeviceDetails& GetDetails() const override;

			virtual bool SetSampleRate(uint32_t sampleRate) override;
			virtual bool SetBufferSize(uint32_t bufferSize) override;

		private:
			HWAVEOUT CreateDevice(uint32_t sampleRate);

			static void CALLBACK SimpleDebugDevice::waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
			void WaveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2);

			void DeviceThread();

			static std::vector<std::vector<float>> SplitDataToSize(int32_t size, const std::vector<float>& data);
		private:
			std::thread m_SampleThread;
			std::vector<float> m_InputData;
			std::vector<std::vector<float>> m_SplitData;
			DeviceDetails m_DeviceDetails;
			bool m_Running;
			std::atomic<int> m_Available;
			HWAVEOUT m_Device;

			std::mutex m_MuxBlock;
			std::condition_variable m_BlockLock;
		};
	}
}