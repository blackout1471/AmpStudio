#include "amppch.h"
#include "LuaFile.h"

namespace AmpProcessing {
	namespace Plugin {

		LuaFile::LuaFile(const std::string& filePath, const std::string& fileContent) : m_LuaContext(std::make_unique<lua_State>()), m_FilePath(filePath),
			m_FileContent(fileContent)
		{
			luaL_openlibs(m_LuaContext.get());

			LOG_INFO("[LUA] has been loaded for {}", filePath);
		}

		LuaFile::~LuaFile()
		{
			lua_close(m_LuaContext.get());
		}

		bool LuaFile::Compile()
		{
			LOG_INFO("[LUA] Compiled the file {}", m_FilePath);
			int success = luaL_dostring(m_LuaContext.get(), m_FileContent.c_str());
			CheckLua(m_LuaContext.get(), success);
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