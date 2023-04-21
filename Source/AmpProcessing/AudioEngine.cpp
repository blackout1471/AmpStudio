#include "amppch.h"
#include "AudioEngine.h"

#include "Effects/AtanDistortion.h"

namespace AmpProcessing {
	void AudioEngine::Init()
	{
		m_EffectProcessors.push_back(std::make_unique<Effects::AtanDistortion>());
		m_EffectProcessors.push_back(std::make_unique<Effects::AtanDistortion>());

		auto device = m_AudioDevice.get();
		auto names = m_AudioDevice->GetDeviceNames();
		m_AudioDevice->Open(names.front());
		m_AudioDevice->SetSampleReadyCallback(std::bind(&AudioEngine::OnSampleReady, this, std::placeholders::_1));
	}

	void AudioEngine::OnSampleReady(std::vector<float>& sample)
	{
		for (size_t i = 0; i < m_EffectProcessors.size(); i++)
		{
			if (!m_EffectProcessors[i]->GetCanProcess())
				continue;

			m_EffectProcessors[i]->Process(sample);
		}
	}
}
