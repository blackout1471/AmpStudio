#pragma once
#include <Utility/File.h>


namespace AmpProcessing {
	namespace Systems {
		class FileWatcherSystem
		{
		public:
			FileWatcherSystem(const std::string pluginPath);
			~FileWatcherSystem();

		private:
			std::vector<Utility::File> m_Files;
			std::string m_PluginPath;
		};
	}
}