#pragma once

namespace AmpProcessing {
	namespace Effects {
		enum class EffectCategory {
			Unknown = 0,
			Distortion,
			Reverb,
			Amp,
			PreAmp,
			Cabinet
		};

		/// <summary>
		/// Mapper method for EffectCategory to get from EffectCategory to it's name in a string.
		/// </summary>
		static std::string ToString(EffectCategory category) {
			switch (category)
			{
			case AmpProcessing::Effects::EffectCategory::Distortion:
				return "Distortion";
				break;
			case AmpProcessing::Effects::EffectCategory::Reverb:
				return "Reverb";
				break;
			case AmpProcessing::Effects::EffectCategory::Amp:
				return "Amp";
				break;
			case AmpProcessing::Effects::EffectCategory::PreAmp:
				return "PreAmp";
				break;
			case AmpProcessing::Effects::EffectCategory::Cabinet:
				return "Cabinet";
				break;
			default:
				return "Unknown";
				break;
			}
		};

		/// <summary>
		/// Returns all the effect categories from the last category given as strings.
		/// </summary>
		static std::vector<std::string> ToStringVector(EffectCategory categoryLast) {
			std::vector<std::string> enumStrings;
			for (size_t i = 0; i < static_cast<int>(categoryLast) + 1; i++)
			{
				enumStrings.push_back(ToString(static_cast<EffectCategory>(i)));
			}

			return enumStrings;
		};
	}
}