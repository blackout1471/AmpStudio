#pragma once
#include <vector>

#define M_PI 3.14159274101257324219f

namespace AmpProcessing {
	namespace Effects {
		class IEffectProcessor {
		public:
			IEffectProcessor() : m_CanProcess(true) {};
			~IEffectProcessor() {};

			virtual void Process(std::vector<float>& sample) = 0;

			inline void ToggleCanProcess() { m_CanProcess = !m_CanProcess; }
			inline const bool GetCanProcess() const { return m_CanProcess; }

		private:
			bool m_CanProcess;
		};
	}
}