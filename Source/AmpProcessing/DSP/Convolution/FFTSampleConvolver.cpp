#include "amppch.h"
#include "FFTSampleConvolver.h"

namespace AmpProcessing {
	namespace DSP {

		bool FFTSampleConvolver::Init(const size_t blockSize, const std::vector<float>& ir)
		{
			return m_Convolver.init(blockSize, &ir.front(), ir.size());
		}

		void FFTSampleConvolver::Process(const std::vector<float>& input, std::vector<float>& output)
		{
			m_Convolver.process(&input.front(), &output.front(), output.size());
		}

		void FFTSampleConvolver::Reset()
		{
			m_Convolver.reset();
		}
	}
}
