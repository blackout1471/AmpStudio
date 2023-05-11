#pragma once
#include "IEffectProcessor.h"
#include "DSP/Convolution/FFTSampleConvolver.h"
#include "Utility/MESA.h"

namespace AmpProcessing {
	namespace Effects {
		class MesaCabinet : public IEffectProcessor {
		public:
			MesaCabinet() : IEffectProcessor("Mesa Cabinet", EffectCategory::Cabinet), m_ProcessBuffer(128), m_SampleConvolver(std::make_unique<DSP::FFTSampleConvolver>()),
				m_Wet(AddParameter(Controls::EffectParameter{ "Wet", 1.0f, 0.f, 0.1f })),
				m_Dry(AddParameter(Controls::EffectParameter{ "Dry", 1.0f, 0.f, 0.1f }))
			{
				auto& outBuffer = MesaFrames;

				m_SampleConvolver->Init(128 * 2, outBuffer);
			};
			~MesaCabinet() {};

			inline virtual void Process(std::vector<float>& sample) override {
				m_SampleConvolver->Process(sample, m_ProcessBuffer);

				// Mix the dry and wet signals according to the wet/dry balance
				for (int i = 0; i < sample.size(); ++i) {
					sample[i] = m_Wet->Value * m_ProcessBuffer[i] + m_Dry->Value * sample[i];
				}
			};

		private:
			std::vector<float> m_ProcessBuffer;
			const std::shared_ptr<const Controls::EffectParameter> m_Wet;
			const std::shared_ptr<const Controls::EffectParameter> m_Dry;
			std::unique_ptr<DSP::ISampleConvolver> m_SampleConvolver;
		};
	}
}