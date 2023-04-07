#pragma once
#include <vector>

namespace AmpProcessing {
	namespace Converters {
		class IAudioConverter {
		public:
			IAudioConverter() = default;
			inline ~IAudioConverter() {};

			virtual void ConvertToFloat(void* from, std::vector<float>& to, unsigned int fromAmount) = 0;
			virtual void ConvertFromFloat(std::vector<float>& from, void* to) = 0;
		};
	}
}