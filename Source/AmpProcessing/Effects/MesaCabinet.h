#pragma once
#include "IEffectProcessor.h"
#include "DSP/Convolution/FFTSampleConvolver.h"
#include "Utility/MESA.h"
#include <cmath>

namespace AmpProcessing {
	namespace Effects {
		class MesaCabinet : public IEffectProcessor {
		public:
			MesaCabinet() : IEffectProcessor("Mesa Cabinet"), m_Processed(128), m_SampleConvolver(std::make_unique<DSP::FFTSampleConvolver>())
			{
				m_SampleConvolver->Init(int(std::pow(2.0, 10)), MesaFrames);
			};
			~MesaCabinet() {};

			inline virtual void Process(std::vector<float>& sample) override {
				m_SampleConvolver->Process(sample, m_Processed);

				sample.assign(m_Processed.begin(), m_Processed.end());
			};
		private:
			std::vector<float> m_Processed;
			std::unique_ptr<DSP::ISampleConvolver> m_SampleConvolver;
		};
	}
}