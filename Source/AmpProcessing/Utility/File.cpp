#include "amppch.h"
#include "File.h"

#include <iostream>
#include <fstream>

namespace AmpProcessing {
	namespace Utility {
		const std::string File::ReadContent(const std::string& path)
		{
			std::ifstream file(path);
			if (!file.is_open()) {
				LOG_ERROR("Could not open file {}", path);
			}
			std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
			file.close();

			return content;
		}
	}
}