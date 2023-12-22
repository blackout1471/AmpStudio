#pragma once
#include <Effects/IEffectProcessor.h>

namespace AmpProcessingWrapper {
	public ref class EffectProcessorWrapper {
	public:
		EffectProcessorWrapper(AmpProcessing::Effects::IEffectProcessor* processor);
		~EffectProcessorWrapper();
		!EffectProcessorWrapper();

		property bool CanProcess {
			bool get();
		}

		property System::String^ Name {
			System::String^ get();
		}

		property AmpProcessing::Effects::EffectCategory Category {
			AmpProcessing::Effects::EffectCategory get();
		}

		void ToggleCanProcess();

	private:
		AmpProcessing::Effects::IEffectProcessor* m_EffectProcessor;
	};
}