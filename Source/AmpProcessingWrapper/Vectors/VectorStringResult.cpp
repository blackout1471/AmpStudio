#include "amppch.h"
#include "VectorStringResult.h"

VectorStringResult ConvertToVectorString(const std::vector<std::string>& vector)
{
	VectorStringResult result{};

	result.count = static_cast<int>(vector.size());

	result.strings = new const char* [result.count];

	for (size_t i = 0; i < result.count; i++)
		result.strings[i] = _strdup(vector[i].c_str());

	return result;
}

VectorStringResult ConvertToVectorString(const std::string& string) {
	VectorStringResult result{};
	result.count = 1;

	result.strings = new const char* [result.count];

	result.strings[0] = _strdup(string.c_str());

	return result;
}

API void FreeVectorStringResult(VectorStringResult result)
{
	for (int i = 0; i < result.count; ++i) {
		free(const_cast<char*>(result.strings[i]));
	}

	delete[] result.strings;
}
