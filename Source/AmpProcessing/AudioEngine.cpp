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
		m_FileWatcher(std::make_unique<Systems::FileWatcherSystem>("Plugins")),
		m_LuaSystem(std::make_unique<Systems::LuaSystem>())
	{}

	AudioEngine::~AudioEngine()
	{
	}

	void AudioEngine::Init()
	{
		//m_LuaSystem->SetStateChangedCallback()

		m_FileWatcher->SetFileStateChangedCallback(std::bind(&AudioEngine::OnFileHasChanged, this, std::placeholders::_1, std::placeholders::_2));	
		m_FileWatcher->Start();

		m_EffectChainSystem->AddAvailableEffect<Effects::AtanDistortion>();
		m_EffectChainSystem->AddAvailableEffect<Effects::MesaCabinet>();
		m_EffectChainSystem->AddAvailableEffect<Effects::ReverbTest>();
		m_EffectChainSystem->AddAvailableEffect<Effects::HardClipper>();

		auto device = m_AudioDevice.get();
		auto names = m_AudioDevice->GetDeviceNames();
		m_AudioDevice->Open(names.front());
		m_AudioDevice->SetSampleReadyCallback(std::bind(&AudioEngine::OnSampleReady, this, std::placeholders::_1));
	}

	void AudioEngine::AddEffectToChain(const std::string& name)
	{
		if (!m_EffectChainSystem->AddEffect(name))
			LOG_INFO("Could not add effect {} to the chain", name);
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

	void AudioEngine::OnFileHasChanged(const Utility::File& file, const Systems::FileWatcherSystem::FileStateChanged state)
	{
		switch (state)
		{
		case Systems::FileWatcherSystem::FileStateChanged::New:
		{
			m_EffectChainSystem->AddAvailableEffect<Effects::LuaEffectProcessor>(file);
			break;
		}
		case Systems::FileWatcherSystem::FileStateChanged::Changed:
		{
			auto wasInEffectChain = m_EffectChainSystem->RemoveEffect(file.GetFileName());
			m_EffectChainSystem->RemoveEffectFromAvailable(file.GetFileName());

			m_EffectChainSystem->AddAvailableEffect<Effects::LuaEffectProcessor>(file);
			if (wasInEffectChain)
				m_EffectChainSystem->AddEffect(file.GetFileName());
		}
		default:
			break;
		};
	}

	void AudioEngine::OnLuaFileHasChanged(Plugins::LuaFile* const lua, const Systems::LuaSystem::StateChanged state)
	{

	}
}
