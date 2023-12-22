#pragma once
#include <AudioEngine.h>

namespace AmpProcessingWrapper {
	public ref class AudioEngineWrapper
	{
	private:
		AmpProcessing::AudioEngine* m_NativeAudioEngine;

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

		System::Collections::Generic::List<System::String^>^ GetAvailableDevices();
	};
}