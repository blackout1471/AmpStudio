#pragma once
#include "Devices/AsioAudioDevice.h"
#include "Systems/EffectChainSystem.h"
#include "Systems/FileWatcherSystem.h"
#include "Systems/LuaSystem.h"

namespace AmpProcessing {
	class AudioEngine
	{
	public:
		AudioEngine(); 
		~AudioEngine();

		void Init();

		const std::vector<Effects::IEffectProcessor*>& GetCurrentEffectChain() const { return m_EffectChainSystem->GetEffectChain(); }
		void AddEffectToChain(const std::string& name);

		const std::vector<std::unique_ptr<Effects::IEffectProcessor>>& GetAvailableEffects() const { return m_EffectChainSystem->GetAvailableEffects(); }

	private:
		void OnSampleReady(std::vector<float>& sample);
		void OnFileHasChanged(const Utility::File& file, const Systems::FileWatcherSystem::FileStateChanged state);
		void OnLuaFileHasChanged(Lua::LuaFile* const lua, const Systems::LuaSystem::StateChanged state);
	private:
		std::unique_ptr<Devices::IAudioDevice> m_AudioDevice;
		std::unique_ptr<Systems::EffectChainSystem> m_EffectChainSystem;
		std::unique_ptr<Systems::FileWatcherSystem> m_FileWatcher;
		std::unique_ptr<Systems::LuaSystem> m_LuaSystem;
	};
}

