#pragma once
#include <vector>
#include <cmath>

namespace AmpProcessing {
	namespace Utility {
		static class AudioUtility {
		public:
			inline static float CalculateDecibelLevel(const std::vector<float>& sample) {
				const float refLevel = 1.0f;
				float sumOfSquares = 0.0f;

				for (size_t i = 0; i < sample.size(); ++i) {
					sumOfSquares += sample[i] * sample[i];
				}

				float rmsLevel = std::sqrt(sumOfSquares / sample.size());
				float dbLevel = 20.0f * std::log10(rmsLevel / refLevel);

				return dbLevel;
			};

		private:
			inline AudioUtility() {};
			inline ~AudioUtility() {};
		};
	}
}