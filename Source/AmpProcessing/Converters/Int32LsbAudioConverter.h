#pragma once
#include "IAudioConverter.h"

namespace AmpProcessing {
	namespace Converters {
		class Int32LsbAudioConverter : public IAudioConverter
		{
		public:
			Int32LsbAudioConverter() = default;
			inline ~Int32LsbAudioConverter() {};

			virtual void ConvertToFloat(void* from, std::vector<float>& to, unsigned int fromAmount) override;
			virtual void ConvertFromFloat(std::vector<float>& from, void* to) override;
		};
	}
}

