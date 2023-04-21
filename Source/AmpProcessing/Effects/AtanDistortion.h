#pragma once
#include "IEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {
		class AtanDistortion : public IEffectProcessor {
		public:
			AtanDistortion() : m_Drive(1), m_Blend(0.6f), m_Range(300), m_Volume(0.7f) {};
			~AtanDistortion() {};

			inline virtual void Process(std::vector<float>& sample) override {
				for (size_t i = 0; i < sample.size(); i++)
				{
					float cleanValue = sample[i];

					sample[i] *= m_Drive * m_Range;

					sample[i] = (((2.f / M_PI) * atan(sample[i]) * m_Blend) + (cleanValue * (1.f - m_Blend)) / 2.f) * m_Volume;
				}
			};
		private:
			float m_Drive;
			float m_Range;
			float m_Volume;
			float m_Blend;
		};
	}
}