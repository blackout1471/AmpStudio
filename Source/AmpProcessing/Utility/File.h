#pragma once
#include <filesystem>

namespace AmpProcessing {
	namespace Utility {
		class File
		{
		public:
			File(const std::string& filePath);
			~File();

			const std::string ReadContent() const;
			bool HasModifiedChanged();

			inline const std::string GetFilePath() const { return m_FilePath.string(); };
			inline const std::string& GetFileName() const { return m_FileName; }
			inline const std::time_t& GetLastModifiedTime() const { return m_LastModifiedTime; }

		private:
			const std::string ReadFileName() const;
			const std::time_t ReadModifiedTime() const;

		private:
			std::filesystem::path m_FilePath;
			std::string m_FileName;

			std::time_t m_LastModifiedTime;
		};
	}
}

