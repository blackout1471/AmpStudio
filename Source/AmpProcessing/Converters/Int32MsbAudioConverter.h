#pragma once
#include "IAudioConverter.h"

namespace AmpProcessing {
	namespace Converters {
		class Int32MsbAudioConverter : public IAudioConverter
		{
		public:
			Int32MsbAudioConverter() = default;
			inline ~Int32MsbAudioConverter() {};

			virtual void ConvertToFloat(void* from, std::vector<float>& to, unsigned int fromAmount) override;
			virtual void ConvertFromFloat(std::vector<float>& from, void* to) override;
		};
	}
}

