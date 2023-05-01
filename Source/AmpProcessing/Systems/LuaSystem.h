#pragma once
#include "Plugins/LuaFile.h"
#include "Utility/File.h"

namespace AmpProcessing {
	namespace Systems {
		class LuaSystem
		{
		public:
			enum StateChanged {
				New,
				Changed,
				Deleted
			};
		public:
			LuaSystem();
			~LuaSystem();

			bool AddLuaFile(const Utility::File& file);

			inline void SetStateChangedCallback(const std::function<void(Plugins::LuaFile* const lua, const LuaSystem::StateChanged state)>& func) { m_LuaStateChangedEvent = func; };
		private:
			bool LuaFileChanged(const Utility::File& file);
			bool LuaFileNew(const Utility::File& file);

			Plugins::LuaFile* GetLuaByFileName(const std::string& name);

		private:
			std::vector<Plugins::LuaFile> m_LuaFiles;
			std::function<void(Plugins::LuaFile* const lua, const LuaSystem::StateChanged state)> m_LuaStateChangedEvent;

		};
	}
}