#pragma once
#include "Devices/AsioAudioDevice.h"
#include "Systems/EffectChainSystem.h"
#include "Systems/FileWatcherSystem.h"

#include "Plugins/LuaFile.h"

namespace AmpProcessing {
	class AudioEngine
	{
	public:
		AudioEngine(); 
		~AudioEngine();

		void Init();

		const std::vector<std::unique_ptr<Effects::IEffectProcessor>>& GetCurrentEffectChain() const { return m_EffectChainSystem->GetEffectChain(); }
		template<typename T, typename... Args>
		inline void AddEffectToChain(Args&&... args) {
			m_EffectChainSystem->AddEffect<T>(std::forward<Args>(args)...);
		};

	private:
		void OnSampleReady(std::vector<float>& sample);
	private:
		std::unique_ptr<Devices::IAudioDevice> m_AudioDevice;
		std::unique_ptr<Systems::EffectChainSystem> m_EffectChainSystem;
		std::unique_ptr<Systems::FileWatcherSystem> m_FileWatcher;
	};
}

