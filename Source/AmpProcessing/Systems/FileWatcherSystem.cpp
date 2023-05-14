#include "amppch.h"
#include "FileWatcherSystem.h"

namespace AmpProcessing {
	namespace Systems {

		FileWatcherSystem::FileWatcherSystem(const std::string pluginPath) : m_PluginPath(pluginPath), m_Files(), m_FileStateChangedEvent(),
			m_RunningThread(false), m_WatchThread()
		{
		}

		FileWatcherSystem::~FileWatcherSystem()
		{
			m_RunningThread = false;
			m_WatchThread->join();
		}

		void FileWatcherSystem::Start()
		{
			m_RunningThread = true;

			if (!std::filesystem::exists(m_PluginPath))
				std::filesystem::create_directories(m_PluginPath);

			GetNewFilesInDirectory();

			m_WatchThread = std::make_unique<std::thread>([&]() {
				while (m_RunningThread) {
					Sleep(1000);

					GetNewFilesInDirectory();
					GetChangesForFiles();
				};
			});
		}

		void FileWatcherSystem::GetNewFilesInDirectory()
		{
			const auto filePaths = GetAllFilesInDirectory(m_PluginPath, ".lua");
			std::vector<Utility::File> files;
			for each (auto & file in filePaths) files.push_back(Utility::File(file.string()));

			auto diff = GetDistinctionFromFiles(files);

			m_Files.insert(m_Files.end(), diff.begin(), diff.end());

			for each (auto& diffFile in diff)
				NewFileFound(diffFile);
		}

		void FileWatcherSystem::GetChangesForFiles()
		{
			for (size_t i = 0; i < m_Files.size(); i++)
				if (m_Files[i].HasModifiedChanged())
					FileModifiedFound(m_Files[i]);
		}

		void FileWatcherSystem::NewFileFound(const Utility::File& file)
		{
			LOG_INFO("[FileWatcher] new file found {}", file.GetFilePath().string());

			if (m_FileStateChangedEvent)
				m_FileStateChangedEvent(file, FileStateChanged::New);
		}

		void FileWatcherSystem::FileModifiedFound(const Utility::File& file)
		{
			LOG_INFO("[FileWatcher] file has been modified {}", file.GetFilePath().string());

			if (m_FileStateChangedEvent)
				m_FileStateChangedEvent(file, FileStateChanged::Changed);
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

			const auto relative_folder = GetWorkingDirectory(path);

			for (const auto& entry : std::filesystem::directory_iterator(relative_folder)) {
				if (std::filesystem::is_directory(entry))
					GetAllFilesInDirectory(entry.path().string(), extension);
				else if (entry.path().extension() == extension)
					paths.push_back(entry.path());
			};

			return paths;
		}

		const std::filesystem::path FileWatcherSystem::GetWorkingDirectory(const std::string& path) const
		{
			auto workingDir = std::filesystem::current_path();
			auto relative_folder = std::filesystem::relative(path, std::filesystem::current_path());
			return relative_folder;
		}
	}
}