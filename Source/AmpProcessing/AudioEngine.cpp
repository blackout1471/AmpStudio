#include "amppch.h"
#include "AudioEngine.h"

#include "Effects/AtanDistortion.h"
#include "Effects/MesaCabinet.h"
#include "Effects/HardClipper.h"

namespace AmpProcessing {
	void AudioEngine::Init()
	{
		AddEffectToChain<Effects::AtanDistortion>();
		AddEffectToChain<Effects::MesaCabinet>();
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
