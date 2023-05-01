#include "amppch.h"
#include "LuaSystem.h"

namespace AmpProcessing {
	namespace Systems {
		LuaSystem::LuaSystem() : m_LuaFiles()
		{
		}

		LuaSystem::~LuaSystem()
		{
		}

		bool LuaSystem::AddLuaFile(const Utility::File& file)
		{
			if (LuaFileChanged(file))
				return true;

			if (LuaFileNew(file))
				return true;

			return false;
		}

		bool LuaSystem::LuaFileChanged(const Utility::File& file)
		{
			auto* lua = GetLuaByFileName(file.GetFileName());
			if (!lua)
				return false;

			lua->Compile(file.ReadContent());
			if (m_LuaStateChangedEvent)
				m_LuaStateChangedEvent(lua, StateChanged::Changed);

			return true;
		}

		bool LuaSystem::LuaFileNew(const Utility::File& file)
		{
			auto lua = Plugins::LuaFile(file.GetFileName(), file.ReadContent());
			lua.Compile();
			m_LuaFiles.push_back(lua);

			if (m_LuaStateChangedEvent)
				m_LuaStateChangedEvent(&lua, StateChanged::New);

			return true;
		}

		Plugins::LuaFile* LuaSystem::GetLuaByFileName(const std::string& name)
		{
			auto it = std::find_if(m_LuaFiles.begin(), m_LuaFiles.end(), [&](const Plugins::LuaFile& lua) { return lua.GetFileName() == name; });
			if (it != m_LuaFiles.end())
				return &(*it);

			return nullptr;
		}
	}
}