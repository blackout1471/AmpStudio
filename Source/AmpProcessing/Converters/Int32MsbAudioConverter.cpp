#include "Int32MsbAudioConverter.h"
namespace AmpProcessing {
	namespace Converters {

		void Int32MsbAudioConverter::ConvertToFloat(void* from, std::vector<float>& to, unsigned int fromAmount)
		{
			int32_t* buffer = (int32_t*)from;
			for (size_t i = 0; i < fromAmount; i++)
			{
				to[i] = static_cast<float>(buffer[i]) / INT32_MAX;
			}
		}

		void Int32MsbAudioConverter::ConvertFromFloat(std::vector<float>& from, void* to)
		{
			auto size = from.size();
			int32_t* buffer = (int32_t*)to;
			for (size_t i = 0; i < size; i++)
			{
				buffer[i] = static_cast<int32_t>(from[i] * INT32_MAX);
			}
		}
	}
}