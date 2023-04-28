#include "amppch.h"
#include "LuaFile.h"

namespace AmpProcessing {
	namespace Plugin {

		LuaFile::LuaFile(const std::string& filePath, const std::string& fileContent) : m_LuaContext(luaL_newstate(), lua_close), m_FilePath(filePath),
			m_FileContent(fileContent)
		{
			luaL_openlibs(m_LuaContext.get());

			LOG_INFO("[LUA] has been loaded for {}", filePath);
		}

		LuaFile::~LuaFile() {}

		bool LuaFile::Compile()
		{
			LOG_INFO("[LUA] Compiled the file {}", m_FilePath);
			int success = luaL_dostring(m_LuaContext.get(), m_FileContent.c_str());
			CheckLua(m_LuaContext.get(), success);

			return success;
		}

		void LuaFile::CallLuaFunction(const std::string& functionName)
		{
			auto* context = m_LuaContext.get();
			lua_getglobal(context, functionName.c_str());

			if (!lua_isfunction(context, -1))
			{
				LOG_WARN("[LUA] {} was not found in file {}", functionName, m_FilePath);
				return;
			}

			CheckLua(context, lua_pcall(context, 0, 1, 0));
		}

		bool LuaFile::CheckLua(lua_State* L, int r)
		{
			if (r != LUA_OK)
			{
				std::string errormsg = lua_tostring(L, -1);
				LOG_ERROR("[LUA] {}", errormsg);

				return false;
			}
			return true;
		}
	}
}