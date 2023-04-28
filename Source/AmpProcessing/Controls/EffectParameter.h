#pragma once
#include <string>

namespace AmpProcessing {
	namespace Controls {
		struct EffectParameter {
			std::string Name;
			float Max;
			float Min;
			float Value;
		};
	}
}