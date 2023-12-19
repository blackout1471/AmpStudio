#pragma once
#include <Global.h>
#include <vector>
#include <string>

struct VectorStringResult {
	const char** strings;
	int count;
};

VectorStringResult ConvertToVectorString(const std::vector<std::string>& vector);
VectorStringResult ConvertToVectorString(const std::string& string);

extern "C" {
	API void FreeVectorStringResult(VectorStringResult result);
}