#include "amppch.h"
#include "EffectChainSystem.h"

namespace AmpProcessing {
	namespace Systems {
		EffectChainSystem::EffectChainSystem()
		{
		}

		EffectChainSystem::~EffectChainSystem()
		{
		}

		bool EffectChainSystem::RemoveEffect(const std::string& name)
		{
			auto it = std::find_if(m_EffectProcessors.begin(), m_EffectProcessors.end(),
				[&name](const std::unique_ptr<Effects::IEffectProcessor>& processor) {
					return processor->GetName() == name;
				});

			if (it != m_EffectProcessors.end()) {
				m_EffectProcessors.erase(it);
				return true;
			}

			return false;
		}
	}
}