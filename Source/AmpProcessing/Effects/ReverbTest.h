#pragma once
#include "IEffectProcessor.h"
#include "DSP/Convolution/FFTSampleConvolver.h"
#include "Utility/MESA.h"
#include "Utility/AudioFile.h"

namespace AmpProcessing {
	namespace Effects {
		class ReverbTest : public IEffectProcessor {
		public:
			ReverbTest() : IEffectProcessor("Reverb Test"), m_ProcessBuffer(128), m_SampleConvolver(std::make_unique<DSP::FFTSampleConvolver>()),
				m_Wet(AddParameter(Controls::EffectParameter{ "Wet", 1.0f, 0.f, 0.1f })),
				m_Dry(AddParameter(Controls::EffectParameter{ "Dry", 1.0f, 0.f, 0.1f }))
			{
				AudioFile<float> in;
				in.load("C:\\Repos\\resources\\greathall.wav");
				auto& outBuffer = in.samples[0];

				m_SampleConvolver->Init(128 * 2, outBuffer);
			};
			~ReverbTest() {};

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