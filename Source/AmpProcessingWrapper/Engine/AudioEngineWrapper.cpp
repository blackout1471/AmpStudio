#include "AudioEngineWrapper.h"
#include <msclr/marshal_cppstd.h>
#include <string>

namespace AmpProcessingWrapper {

	AudioEngineWrapper::AudioEngineWrapper()
	{
		m_NativeAudioEngine = new AmpProcessing::AudioEngine();
	}

	AudioEngineWrapper::~AudioEngineWrapper()
	{
		delete m_NativeAudioEngine;
	}

	AudioEngineWrapper::!AudioEngineWrapper()
	{
		delete m_NativeAudioEngine;
	}

	void AudioEngineWrapper::Init()
	{
		m_NativeAudioEngine->Init();
	}

	void AudioEngineWrapper::SetDesiredInputDbLevel(int desiredLevel)
	{
		m_NativeAudioEngine->SetDesiredInputDbLevel(desiredLevel);
	}

	void AudioEngineWrapper::SetDesiredOutputDbLevel(int desiredLevel)
	{
		m_NativeAudioEngine->SetDesiredOutputDbLevel(desiredLevel);
	}

	System::Collections::Generic::List<System::String^>^ AudioEngineWrapper::GetAvailableDevices()
	{
		auto cliList = gcnew System::Collections::Generic::List<System::String^>();

		auto& devices = m_NativeAudioEngine->GetAvailableDevices();

		for each (const auto& device in devices)
		{
			auto cliString = msclr::interop::marshal_as<System::String^>(device);

			cliList->Add(cliString);
		}

		return cliList;
	}

	float AudioEngineWrapper::InputDbLevel::get()
	{
		return m_NativeAudioEngine->GetInputDbLevel();
	}

	float AudioEngineWrapper::OutputDbLevel::get()
	{
		return m_NativeAudioEngine->GetOutputDbLevel();
	}

	float AudioEngineWrapper::DesiredInputDbLevel::get()
	{
		return m_NativeAudioEngine->GetDesiredInputDbLevel();
	}

	float AudioEngineWrapper::DesiredOutputDbLevel::get()
	{
		return m_NativeAudioEngine->GetDesiredOutputDbLevel();
	}

}