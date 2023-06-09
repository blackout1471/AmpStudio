#pragma once
#include "Lua/LuaFile.h"
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

			inline void SetStateChangedCallback(const std::function<void(Lua::LuaFile* const lua, const LuaSystem::StateChanged state)>& func) { m_LuaStateChangedEvent = func; };
		private:
			bool LuaFileChanged(const Utility::File& file);
			bool LuaFileNew(const Utility::File& file);

			Lua::LuaFile* GetLuaByFileName(const std::string& name);

		private:
			std::vector<std::unique_ptr<Lua::LuaFile>> m_LuaFiles;
			std::function<void(Lua::LuaFile* const lua, const LuaSystem::StateChanged state)> m_LuaStateChangedEvent;

		};
	}
}