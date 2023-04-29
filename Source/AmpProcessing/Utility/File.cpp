#include "amppch.h"
#include "File.h"
#include <fstream>

namespace AmpProcessing {
	namespace Utility {
		File::File(const std::string& filePath) : m_FilePath(filePath), m_FileName(), m_LastModifiedTime()
		{
			m_FileName = ReadFileName();
			m_LastModifiedTime = ReadModifiedTime();
		}

		File::~File()
		{
		}

		const std::string File::ReadContent() const
		{
			auto filePath = m_FilePath.string();
			std::ifstream file(filePath);
			if (!file.is_open()) {
				LOG_ERROR("Could not open file {}", filePath);
			}
			std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
			file.close();

			return content;
		}

		const std::string File::ReadFileName() const
		{
			return m_FilePath
				.filename()
				.string();
		}

		const std::time_t File::ReadModifiedTime() const
		{
			auto last_write_time = std::filesystem::last_write_time(m_FilePath);

			std::chrono::time_point<std::chrono::system_clock> time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(last_write_time.time_since_epoch()));

			return std::chrono::system_clock::to_time_t(time_point);
		}
	}
}