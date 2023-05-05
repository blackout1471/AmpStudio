#pragma once
#include "Effects/IEffectProcessor.h"

namespace AmpProcessing {
	namespace Systems {
		class EffectChainSystem
		{
		public:
			EffectChainSystem();
			~EffectChainSystem();

			bool AddEffect(const std::string& name);
			bool RemoveEffect(const std::string& name);
			inline std::vector<Effects::IEffectProcessor*>& GetEffectChain() { return m_EffectProcessors; }

			template<typename T, typename... Args>
			inline std::enable_if_t<std::is_base_of_v<Effects::IEffectProcessor, T>, T*> AddAvailableEffect(Args&&... args) {
				auto effect = std::make_unique<T>(std::forward<Args>(args)...);
				m_AvailableEffects.push_back(std::move(effect));
				return effect.get();
			};
			bool RemoveEffectFromAvailable(const std::string& name);
			inline std::vector<std::unique_ptr<Effects::IEffectProcessor>>& GetAvailableEffects() { return m_AvailableEffects; }

		private:
			std::vector<Effects::IEffectProcessor*> m_EffectProcessors;
			std::vector<std::unique_ptr<Effects::IEffectProcessor>> m_AvailableEffects;
		};
	}
}