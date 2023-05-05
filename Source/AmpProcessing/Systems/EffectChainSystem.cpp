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

		bool EffectChainSystem::AddEffect(const std::string& name)
		{
			// TODO: As of right now it's not possible to add the same effect more than once...
			// Because the available list and the chain list is 1 -> 1. The effect chain list has reference pointer to available list.
			auto effectit = std::find_if(m_EffectProcessors.begin(), m_EffectProcessors.end(),
				[&name](const Effects::IEffectProcessor* processor) {
					return processor->GetName() == name;
				});

			if (effectit != m_EffectProcessors.end())
				return false;

			auto it = std::find_if(m_AvailableEffects.begin(), m_AvailableEffects.end(),
				[&](const auto& effect) { return effect->GetName() == name; });
			if (it == m_AvailableEffects.end()) {
				return false;
			}

			m_EffectProcessors.push_back(it->get());
			return true;
		}

		bool EffectChainSystem::RemoveEffect(const std::string& name)
		{
			auto it = std::find_if(m_EffectProcessors.begin(), m_EffectProcessors.end(),
				[&name](const Effects::IEffectProcessor* processor) {
					return processor->GetName() == name;
				});

			if (it != m_EffectProcessors.end()) {
				m_EffectProcessors.erase(it);
				return true;
			}

			return false;
		}

		bool EffectChainSystem::RemoveEffectFromAvailable(const std::string& name)
		{
			auto it = std::find_if(m_AvailableEffects.begin(), m_AvailableEffects.end(),
				[&name](const std::unique_ptr<Effects::IEffectProcessor>& processor) {
					return processor->GetName() == name;
				});

			if (it != m_AvailableEffects.end()) {
				m_AvailableEffects.erase(it);
				return true;
			}

			return false;
		}
	}
}