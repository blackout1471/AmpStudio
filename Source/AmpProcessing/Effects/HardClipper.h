#pragma once
#include "IEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {
		class HardClipper : public IEffectProcessor {
		public:
			HardClipper() : IEffectProcessor("Hard clipper") {};
			~HardClipper() {};

			inline virtual void Process(std::vector<float>& sample) override {
				for (size_t i = 0; i < sample.size(); i++)
				{
					if (sample[i] > 1.f)
						sample[i] = 1.f;
					
					if (sample[i] < -1.f)
						sample[i] = -1.f;
				}
			};
		};
	}
}