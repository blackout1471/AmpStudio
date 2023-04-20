#pragma once
#include <vector>

namespace AmpProcessing {
	namespace DSP {
		class ISampleConvolver
		{
		public:
			ISampleConvolver() {};
			inline ~ISampleConvolver() {};

			virtual bool Init(const size_t blockSize, const std::vector<float>& ir) = 0;
			virtual void Process(const std::vector<float>& input, std::vector<float>& output) = 0;
			virtual void Reset() = 0;
		};
	}
}