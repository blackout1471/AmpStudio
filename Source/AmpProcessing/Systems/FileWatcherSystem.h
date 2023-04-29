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

			void GetNewFilesInDirectory();

		private:
			void NewFileFound(const Utility::File& file);

			const std::vector<Utility::File> GetDistinctionFromFiles(std::vector<Utility::File>& files);
			const std::vector<std::filesystem::path> GetAllFilesInDirectory(const std::string& path, const std::string& extension) const;

		private:
			std::vector<Utility::File> m_Files;
			std::string m_PluginPath;

			std::function<void(const Utility::File&, const FileStateChanged state)> m_FileStateChangedCallback;
		};
	}
}