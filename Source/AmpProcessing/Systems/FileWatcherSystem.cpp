#include "amppch.h"
#include "FileWatcherSystem.h"

namespace AmpProcessing {
	namespace Systems {

		FileWatcherSystem::FileWatcherSystem(const std::string pluginPath) : m_PluginPath(pluginPath), m_Files(), m_FileStateChangedCallback()
		{
		}

		FileWatcherSystem::~FileWatcherSystem()
		{
		}

		void FileWatcherSystem::GetNewFilesInDirectory()
		{
			const auto filePaths = GetAllFilesInDirectory(m_PluginPath, ".lua");
			std::vector<Utility::File> files;
			for each (auto & file in filePaths) files.push_back(Utility::File(file.string()));

			auto diff = GetDistinctionFromFiles(files);

			m_Files.insert(m_Files.end(), diff.begin(), diff.end());

			for each (auto & diffFile in diff)
				NewFileFound(diffFile);
		}

		void FileWatcherSystem::NewFileFound(const Utility::File& file)
		{
			LOG_INFO("[FileWatcher] new file found {}", file.GetFilePath());

			if (m_FileStateChangedCallback)
				m_FileStateChangedCallback(file, FileStateChanged::New);
		}

		const std::vector<Utility::File> FileWatcherSystem::GetDistinctionFromFiles(std::vector<Utility::File>& files)
		{
			std::vector<Utility::File> diff;
			
			std::sort(files.begin(), files.end(), [](const Utility::File& a, const Utility::File& b) { return a.GetFileName() < b.GetFileName(); });
			std::sort(m_Files.begin(), m_Files.end(), [](const Utility::File& a, const Utility::File& b) { return a.GetFileName() < b.GetFileName(); });

			std::set_difference(files.begin(), files.end(), m_Files.begin(), m_Files.end(), std::back_inserter(diff),
				[](const Utility::File& a, const Utility::File& b) { return a.GetFileName() < b.GetFileName(); });

			return diff;
		}

		const std::vector<std::filesystem::path> FileWatcherSystem::GetAllFilesInDirectory(const std::string& path, const std::string& extension) const
		{
			std::vector<std::filesystem::path> paths;

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				if (std::filesystem::is_directory(entry))
					GetAllFilesInDirectory(entry.path().string(), extension);
				else if (entry.path().extension() == extension)
					paths.push_back(entry.path());
			}

			return paths;
		}
	}
}