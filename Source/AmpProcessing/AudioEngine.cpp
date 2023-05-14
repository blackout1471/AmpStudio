#include "amppch.h"
#include "AudioEngine.h"

#include "Devices/DirectSoundDebugDevice.h"

#include "Effects/AtanDistortion.h"
#include "Effects/MesaCabinet.h"
#include "Effects/LuaEffectProcessor.h"

#include "Utility/AudioUtility.h"

namespace AmpProcessing {
	AudioEngine::AudioEngine() : m_AudioDevice(std::make_unique<Devices::DirectSoundDebugDevice>()),
		m_EffectChainSystem(std::make_unique<Systems::EffectChainSystem>()),
		m_FileWatcher(std::make_unique<Systems::FileWatcherSystem>("Plugins")),
		m_LuaSystem(std::make_unique<Systems::LuaSystem>()), 
		m_InputDbLevel(-60), m_OutputDbLevel(-60), m_DesiredInputDbLevel(0), m_DesiredOutputDbLevel(0)
	{}

	AudioEngine::~AudioEngine()
	{
	}

	void AudioEngine::Init()
	{
		m_LuaSystem->SetStateChangedCallback(std::bind(&AudioEngine::OnLuaFileHasChanged, this, std::placeholders::_1, std::placeholders::_2));

		m_FileWatcher->SetFileStateChangedCallback(std::bind(&AudioEngine::OnFileHasChanged, this, std::placeholders::_1, std::placeholders::_2));	
		m_FileWatcher->Start();

		m_EffectChainSystem->AddAvailableEffect<Effects::AtanDistortion>();
		m_EffectChainSystem->AddAvailableEffect<Effects::MesaCabinet>();

		auto device = m_AudioDevice.get();
		auto names = m_AudioDevice->GetDeviceNames();
		SetupDevice(names.front());
	}

	void AudioEngine::SetupDevice(const std::string& name)
	{
		m_AudioDevice->Open(name);
		m_AudioDevice->SetSampleReadyCallback(std::bind(&AudioEngine::OnSampleReady, this, std::placeholders::_1));
	}

	void AudioEngine::AddEffectToChain(const std::string& name)
	{
		if (!m_EffectChainSystem->AddEffect(name))
			LOG_INFO("Could not add effect {} to the chain", name);
	}

	bool AudioEngine::SetNewDevice(const std::string& name)
	{
		m_AudioDevice.reset();
		m_AudioDevice = std::make_unique<Devices::AsioAudioDevice>();
		SetupDevice(name);

		return true;
	}

	void AudioEngine::OnSampleReady(std::vector<float>& sample)
	{
		auto& effects = m_EffectChainSystem->GetEffectChain();

		Utility::AudioUtility::AdjustToDecibelLevel(m_DesiredInputDbLevel, sample);
		m_InputDbLevel = Utility::AudioUtility::CalculateDecibelLevel(sample);

		for (size_t i = 0; i < effects.size(); i++)
		{
			if (!effects[i]->GetCanProcess())
				continue;

			effects[i]->Process(sample);
		}

		Utility::AudioUtility::AdjustToDecibelLevel(m_DesiredOutputDbLevel, sample);
		m_OutputDbLevel = Utility::AudioUtility::CalculateDecibelLevel(sample);
	}

	void AudioEngine::OnFileHasChanged(const Utility::File& file, const Systems::FileWatcherSystem::FileStateChanged state)
	{
		switch (state)
		{
		case Systems::FileWatcherSystem::FileStateChanged::New:
		{
			m_LuaSystem->AddLuaFile(file);
			break;
		}
		case Systems::FileWatcherSystem::FileStateChanged::Changed:
		{
			m_LuaSystem->AddLuaFile(file);
			break;
		}
		default:
			break;
		};
	}

	void AudioEngine::OnLuaFileHasChanged(Lua::LuaFile* const lua, const Systems::LuaSystem::StateChanged state)
	{
		switch (state)
		{
		case Systems::LuaSystem::New:
		{
			m_EffectChainSystem->AddAvailableEffect<Effects::LuaEffectProcessor>(lua);
			break;
		}
		case Systems::LuaSystem::Changed:
		{
			auto existsInChain = m_EffectChainSystem->RemoveEffect(lua->GetFileName());
			m_EffectChainSystem->RemoveEffectFromAvailable(lua->GetFileName());
			m_EffectChainSystem->AddAvailableEffect<Effects::LuaEffectProcessor>(lua);
			if (existsInChain)
				m_EffectChainSystem->AddEffect(lua->GetFileName());

			break;
		}
		default:
			break;
		}
	}
}
