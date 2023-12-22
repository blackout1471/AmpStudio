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

	bool AudioEngineWrapper::SetSampleRate(uint32_t sampleRate)
	{
		return m_NativeAudioEngine->SetSampleRate(sampleRate);
	}

	bool AudioEngineWrapper::SetBufferSize(uint32_t bufferSize)
	{
		return m_NativeAudioEngine->SetBufferSize(bufferSize);
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

	DeviceDetailsWrapper^ AudioEngineWrapper::GetDeviceDetails()
	{
		const auto& nativeDetails = m_NativeAudioEngine->GetDeviceDetails();

		DeviceDetailsWrapper^ managedDetails = gcnew DeviceDetailsWrapper();
		managedDetails->Name = gcnew System::String(nativeDetails.name.c_str());
		managedDetails->InputChannels = nativeDetails.inputChannels;
		managedDetails->OutputChannels = nativeDetails.outputChannels;
		managedDetails->MinBufferSize = nativeDetails.minBufferSize;
		managedDetails->MaxBufferSize = nativeDetails.maxBufferSize;
		managedDetails->PreferredBufferSize = nativeDetails.prefferedBufferSize;
		managedDetails->GranularityBuffer = nativeDetails.granularityBuffer;
		managedDetails->SampleRate = nativeDetails.sampleRate;

		return managedDetails;
	}

	System::Collections::Generic::List<EffectProcessorWrapper^>^ AudioEngineWrapper::GetAvailableEffects()
	{
		auto cliList = gcnew System::Collections::Generic::List<EffectProcessorWrapper^>();

		auto& effects = m_NativeAudioEngine->GetAvailableEffects();

		for each (auto & effect in effects)
			cliList->Add(gcnew EffectProcessorWrapper(effect.get()));

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