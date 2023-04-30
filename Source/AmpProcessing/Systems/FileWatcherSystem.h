#pragma once
#include <Utility/File.h>


namespace AmpProcessing {
	namespace Systems {
		enum FileStateChanged {
			New,
			Changed,
			Deleted
		};

		class FileWatcherSystem
		{
		public:
			FileWatcherSystem(const std::string pluginPath);
			~FileWatcherSystem();

			void Start();

			void GetNewFilesInDirectory();
			void GetChangesForFiles();

			inline void SetFileStateChangedCallback(const std::function<void(const Utility::File& file, const FileStateChanged state)>& callback) {
				m_FileStateChangedEvent = callback;
			};

		private:
			void NewFileFound(const Utility::File& file);
			void FileModifiedFound(const Utility::File& file);

			const std::vector<Utility::File> GetDistinctionFromFiles(std::vector<Utility::File>& files);
			const std::vector<std::filesystem::path> GetAllFilesInDirectory(const std::string& path, const std::string& extension) const;

		private:
			std::vector<Utility::File> m_Files;
			std::string m_PluginPath;
			bool m_RunningThread;
			std::unique_ptr<std::thread> m_WatchThread;

			std::function<void(const Utility::File&, const FileStateChanged state)> m_FileStateChangedEvent;
		};
	}
}