#pragma once
#include <vector>

namespace AmpProcessing {
	namespace DSP {
		/// <summary>
		/// The ISample convolver is a contract for how a convolver should behave.
		/// </summary>
		class ISampleConvolver
		{
		public:
			ISampleConvolver() {};
			inline ~ISampleConvolver() {};

			/// <summary>
			/// The unit function will setup the necessary data & ready the convolution process.
			/// </summary>
			/// <param name="blockSize">- The size to split the blocks into.</param>
			/// <param name="ir">- The impulse response to use.</param>
			/// <returns>Whether the initialization was successfull.</returns>
			virtual bool Init(const size_t blockSize, const std::vector<float>& ir) = 0;

			/// <summary>
			/// Called to process the next bit of data.
			/// The method will use output's size as a mean to see how much data is needed.
			/// </summary>
			/// <param name="input">- The input data to convolve.</param>
			/// <param name="output">- Output is the vector to insert the data.</param>
			virtual void Process(const std::vector<float>& input, std::vector<float>& output) = 0;

			/// <summary>
			/// Resets all the data required by the converter.
			/// </summary>
			virtual void Reset() = 0;
		};
	}
}