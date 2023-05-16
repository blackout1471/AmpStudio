#pragma once
#include <string>
#include "EffectCategory.h"

namespace AmpProcessing {
	namespace Effects {
		struct EffectProcessorMetaData {
			std::string Name;
			EffectCategory Category;
		};
	}
}