#include "amppch.h"
#include "AudioEngine.h"

#include "Effects/AtanDistortion.h"
#include "Effects/MesaCabinet.h"
#include "Effects/HardClipper.h"
#include "Effects/ReverbTest.h"
#include "Effects/LuaEffectProcessor.h"

namespace AmpProcessing {
	AudioEngine::AudioEngine() : m_AudioDevice(std::make_unique<Devices::AsioAudioDevice>()),
		m_EffectChainSystem(std::make_unique<Systems::EffectChainSystem>()),
		m_FileWatcher(std::make_unique<Systems::FileWatcherSystem>("C:\\Repos\\resources\\"))
	{}

	AudioEngine::~AudioEngine()
	{
	}

	void AudioEngine::Init()
	{
		m_FileWatcher->SetFileStateChangedCallback([&](const Utility::File& file, const Systems::FileStateChanged state) {

			switch (state)
			{
			case Systems::FileStateChanged::New:
			{
				m_EffectChainSystem->AddEffect<Effects::LuaEffectProcessor>(file);
				break;
			}
			case Systems::FileStateChanged::Changed:
			{
				if (m_EffectChainSystem->RemoveEffect(file.GetFileName()))
					m_EffectChainSystem->AddEffect<Effects::LuaEffectProcessor>(file);
				break;
			}
			default:
				break;
			};
		});
		m_FileWatcher->Start();

		AddEffectToChain<Effects::AtanDistortion>();
		AddEffectToChain<Effects::MesaCabinet>();
		AddEffectToChain<Effects::ReverbTest>();
		AddEffectToChain<Effects::HardClipper>();

		auto device = m_AudioDevice.get();
		auto names = m_AudioDevice->GetDeviceNames();
		m_AudioDevice->Open(names.front());
		m_AudioDevice->SetSampleReadyCallback(std::bind(&AudioEngine::OnSampleReady, this, std::placeholders::_1));
	}

	void AudioEngine::OnSampleReady(std::vector<float>& sample)
	{
		auto& effects = m_EffectChainSystem->GetEffectChain();

		for (size_t i = 0; i < effects.size(); i++)
		{
			if (!effects[i]->GetCanProcess())
				continue;

			effects[i]->Process(sample);
		}
	}
}
