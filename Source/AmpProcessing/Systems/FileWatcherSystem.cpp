#include "amppch.h"
#include "FileWatcherSystem.h"

namespace AmpProcessing {
	namespace Systems {

		FileWatcherSystem::FileWatcherSystem(const std::string pluginPath) : m_PluginPath(pluginPath)
		{
		}

		FileWatcherSystem::~FileWatcherSystem()
		{
		}
	}
}