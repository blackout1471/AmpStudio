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

		inline const float GetInputDbLevel() const { return m_InputDbLevel; }
		inline const float GetOutputDbLevel() const { return m_OutputDbLevel; }

		inline void SetDesiredInputDbLevel(int desiredLevel) { m_DesiredInputDbLevel = desiredLevel; }
		inline void SetDesiredOutputDbLevel(int desiredLevel) { m_DesiredOutputDbLevel = desiredLevel; }

		inline const std::vector<std::string> GetAvailableDevices() const { return m_AudioDevice->GetDeviceNames(); }

		inline const Devices::DeviceDetails& GetDeviceDetails() const { return m_AudioDevice->GetDetails(); }

	private:
		void OnSampleReady(std::vector<float>& sample);
		void OnFileHasChanged(const Utility::File& file, const Systems::FileWatcherSystem::FileStateChanged state);
		void OnLuaFileHasChanged(Lua::LuaFile* const lua, const Systems::LuaSystem::StateChanged state);
	private:
		std::unique_ptr<Devices::IAudioDevice> m_AudioDevice;
		std::unique_ptr<Systems::EffectChainSystem> m_EffectChainSystem;
		std::unique_ptr<Systems::FileWatcherSystem> m_FileWatcher;
		std::unique_ptr<Systems::LuaSystem> m_LuaSystem;

		// TODO: Move db calculation & getters into device
		float m_InputDbLevel;
		float m_OutputDbLevel;

		int m_DesiredInputDbLevel;
		int m_DesiredOutputDbLevel;
	};
}

