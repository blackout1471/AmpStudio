#pragma once
#include "Effects/IEffectProcessor.h"

namespace AmpProcessing {
	namespace Systems {
		class EffectSystem
		{
		public:
			EffectSystem();
			~EffectSystem();

		private:
			std::vector<std::unique_ptr<Effects::IEffectProcessor>> m_EffectProcessors;
		};
	}
}