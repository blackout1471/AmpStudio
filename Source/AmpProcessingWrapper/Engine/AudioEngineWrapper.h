#pragma once
#include <AudioEngine.h>
#include "DeviceDetailsWrapper.h"
#include "EffectProcessorWrapper.h"

namespace AmpProcessingWrapper {
	public ref class AudioEngineWrapper
	{
	public:
		AudioEngineWrapper();
		~AudioEngineWrapper();
		!AudioEngineWrapper();

		void Init();

		property float InputDbLevel {
			float get();
		}

		property float OutputDbLevel {
			float get();
		}

		property float DesiredInputDbLevel {
			float get();
		}

		property float DesiredOutputDbLevel {
			float get();
		}

		void SetDesiredInputDbLevel(int desiredLevel);
		void SetDesiredOutputDbLevel(int desiredLevel);

		bool SetSampleRate(uint32_t sampleRate);
		bool SetBufferSize(uint32_t bufferSize);

		System::Collections::Generic::List<System::String^>^ GetAvailableDevices();

		DeviceDetailsWrapper^ GetDeviceDetails();

		System::Collections::Generic::List<EffectProcessorWrapper^>^ GetAvailableEffects();

	private:
		AmpProcessing::AudioEngine* m_NativeAudioEngine;
	};
}