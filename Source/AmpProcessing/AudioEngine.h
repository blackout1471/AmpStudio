#pragma once
#include "Devices/AsioAudioDevice.h"
#include "Effects/IEffectProcessor.h"

namespace AmpProcessing {
	class AudioEngine
	{
	public:
		AudioEngine() : m_AudioDevice(std::make_unique<Devices::AsioAudioDevice>()), m_EffectProcessors()
		{};
		~AudioEngine() {};

		void Init();

		const std::vector<std::unique_ptr<Effects::IEffectProcessor>>& GetEffectsProcessors() const { return m_EffectProcessors; }

	private:
		void OnSampleReady(std::vector<float>& sample);
	private:
		std::unique_ptr<Devices::IAudioDevice> m_AudioDevice;
		std::vector<std::unique_ptr<Effects::IEffectProcessor>> m_EffectProcessors;
	};
}

