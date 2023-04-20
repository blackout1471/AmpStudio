#pragma once
#include "ISampleConvolver.h"
#include <FFTConvolver.h>

namespace AmpProcessing {
	namespace DSP {
		class FFTSampleConvolver : public ISampleConvolver
		{
		public:
			FFTSampleConvolver() : m_Convolver() {};
			inline ~FFTSampleConvolver() {};

			virtual bool Init(const size_t blockSize, const std::vector<float>& ir) override;
			virtual void Process(const std::vector<float>& input, std::vector<float>& output) override;
			virtual void Reset() override;

		private:
			fftconvolver::FFTConvolver m_Convolver;
		};
	}
}