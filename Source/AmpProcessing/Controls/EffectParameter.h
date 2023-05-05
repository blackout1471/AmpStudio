#pragma once
#include <string>

namespace AmpProcessing {
	namespace Controls {
		/// <summary>
		/// Effect parameter defines a type which can be used by plugins to use for processing
		/// </summary>
		struct EffectParameter {
			std::string Name;
			float Max;
			float Min;
			float Value;
		};
	}
}