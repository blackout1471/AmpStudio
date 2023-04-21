#pragma once
#include "IEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {
		class AtanDistortion : public IEffectProcessor {
		public:
			AtanDistortion() : IEffectProcessor("Atan Distortion") 
			{
				m_Drive = { "Drive", 1.0f, 0.f, 1.f }; AddParameter(m_Drive);
				m_Range = { "Range", 3000.0f, 0.f, 300.f }; AddParameter(m_Range);
				m_Volume = { "Volume", 1.0f, 0.f, 0.7f }; AddParameter(m_Volume);
				m_Blend = { "Blend", 1.0f, 0.f, 0.6f }; AddParameter(m_Blend);
			};
			~AtanDistortion() {};

			inline virtual void Process(std::vector<float>& sample) override {
				for (size_t i = 0; i < sample.size(); i++)
				{
					float cleanValue = sample[i];

					sample[i] *= m_Drive.Value * m_Range.Value;

					sample[i] = (((2.f / M_PI) * atan(sample[i]) * m_Blend.Value) + (cleanValue * (1.f - m_Blend.Value)) / 2.f) * m_Volume.Value;
				}
			};
		private:
			Controls::EffectParameter m_Drive;
			Controls::EffectParameter m_Range;
			Controls::EffectParameter m_Volume;
			Controls::EffectParameter m_Blend;
		};
	}
}