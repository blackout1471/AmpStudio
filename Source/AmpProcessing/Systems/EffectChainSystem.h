#pragma once
#include "Effects/IEffectProcessor.h"

namespace AmpProcessing {
	namespace Systems {
		class EffectChainSystem
		{
		public:
			EffectChainSystem();
			~EffectChainSystem();

			template<typename T, typename... Args>
			inline std::enable_if_t<std::is_base_of_v<Effects::IEffectProcessor, T>, T*> AddEffect(Args&&... args) {
				auto effect = std::make_unique<T>(std::forward<Args>(args)...);
				m_EffectProcessors.push_back(std::move(effect));
				return effect.get();
			};

			bool RemoveEffect(const std::string& name);

			inline std::vector<std::unique_ptr<Effects::IEffectProcessor>>& GetEffectChain() { return m_EffectProcessors; }

		private:
			std::vector<std::unique_ptr<Effects::IEffectProcessor>> m_EffectProcessors;
		};
	}
}