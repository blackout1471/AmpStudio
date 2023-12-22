#pragma once
#include <AudioEngine.h>

public ref class ManagedAudioEngine
{
private:
	AmpProcessing::AudioEngine* m_NativeAudioEngine;

public:
	ManagedAudioEngine();
	~ManagedAudioEngine();
	!ManagedAudioEngine();

	void Init();

	property float InputDbLevel {
		float get();
	}

	property float OutputDbLevel {
		float get();
	}

	void SetDesiredInputDbLevel(int desiredLevel);
	void SetDesiredOutputDbLevel(int desiredLevel);

	//System::Collections::Generic::List<System::String^>^ GetAvailableDevices();
};