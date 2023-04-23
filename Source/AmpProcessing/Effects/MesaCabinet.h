#pragma once
#include "IEffectProcessor.h"
#include "DSP/Convolution/FFTSampleConvolver.h"
#include "Utility/AudioFile.h"
#include <cmath>

namespace AmpProcessing {
	namespace Effects {
		class MesaCabinet : public IEffectProcessor {
		public:
			MesaCabinet() : IEffectProcessor("Mesa Cabinet"), m_ProcessBuffer(128), m_SampleConvolver(std::make_unique<DSP::FFTSampleConvolver>()),
				m_Wet(AddParameter(Controls::EffectParameter{ "Wet", 1.0f, 0.f, 0.1f })),
				m_Dry(AddParameter(Controls::EffectParameter{ "Dry", 1.0f, 0.f, 0.1f }))
			{
				AudioFile<float> cabFile;
				cabFile.load("C:\\Repos\\resources\\cabs\\57_1_inch_cap_pres_2.wav");
				auto& outBuffer = cabFile.samples[0];

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
			inline float resampleSample(const std::vector<float>& buffer, float phaseIncrement, float phase) {
				int index = ((int)phase + buffer.size()) % buffer.size();
				float frac = phase - index;
				float s0 = buffer[index];
				float s1 = buffer[(index + 1) % buffer.size()];
				float resampled = s0 + frac * (s1 - s0);
				phase += phaseIncrement;
				while (phase >= buffer.size()) {
					phase -= buffer.size();
				}
				return resampled;
			}

			void resampleBuffer(const std::vector<float>& inBuffer, std::vector<float>& outBuffer, float ratio) {
				float phaseIncrement = inBuffer.size() / (float)outBuffer.size() * ratio;
				float phase = 0.0f;
				for (int i = 0; i < outBuffer.size(); i++) {
					outBuffer[i] = resampleSample(inBuffer, phaseIncrement, phase);
					phase += phaseIncrement;
				}
			}
		private:
			std::vector<float> m_ProcessBuffer;
			const std::shared_ptr<const Controls::EffectParameter> m_Wet;
			const std::shared_ptr<const Controls::EffectParameter> m_Dry;
			std::unique_ptr<DSP::ISampleConvolver> m_SampleConvolver;
		};
	}
}