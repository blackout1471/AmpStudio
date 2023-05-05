#pragma once
#include <vector>

namespace AmpProcessing {
	namespace Converters {
		/// <summary>
		/// Audio converter interface used for convertiong audio samples to and from specific formats.
		/// </summary>
		class IAudioConverter {
		public:
			IAudioConverter() = default;
			inline ~IAudioConverter() {};

			/// <summary>
			/// Converts a array of data to float values.
			/// </summary>
			/// <param name="from">- The data to convert from</param>
			/// <param name="to">- The output vector.</param>
			/// <param name="fromAmount">- The size of the from array</param>
			virtual void ConvertToFloat(void* from, std::vector<float>& to, unsigned int fromAmount) = 0;

			/// <summary>
			/// Converts from float back to the original values.
			/// </summary>
			/// <param name="from">- The data which should be converted.</param>
			/// <param name="to">- The original data pointer, which the converted will be inputted to.</param>
			virtual void ConvertFromFloat(std::vector<float>& from, void* to) = 0;
		};
	}
}