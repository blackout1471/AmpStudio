#pragma once
#include "IEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {
		class AtanDistortion : public IEffectProcessor {
		public:
			AtanDistortion() : IEffectProcessor("Atan Distortion"), 
				m_Drive(AddParameter(Controls::EffectParameter{"Drive", 1.0f, 0.f, 1.f})),
				m_Range(AddParameter(Controls::EffectParameter{"Range", 3000.0f, 0.f, 300.f})),
				m_Volume(AddParameter(Controls::EffectParameter{ "Volume", 1.0f, 0.f, 0.7f })),
				m_Blend(AddParameter(Controls::EffectParameter{ "Blend", 1.0f, 0.f, 0.6f }))
			{
			};
			~AtanDistortion() {};

			inline virtual void Process(std::vector<float>& sample) override {
				for (size_t i = 0; i < sample.size(); i++)
				{
					float cleanValue = sample[i];

					sample[i] *= m_Drive->Value * m_Range->Value;

					sample[i] = (((2.f / M_PI) * atan(sample[i]) * m_Blend->Value) + (cleanValue * (1.f - m_Blend->Value)) / 2.f) * m_Volume->Value;
				}
			};
		private:
			const std::shared_ptr<const Controls::EffectParameter> m_Drive;
			const std::shared_ptr<const Controls::EffectParameter> m_Range;
			const std::shared_ptr<const Controls::EffectParameter> m_Volume;
			const std::shared_ptr<const Controls::EffectParameter> m_Blend;
		};
	}
}