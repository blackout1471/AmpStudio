#include "amppch.h"
#include "LuaFile.h"

#include <sstream>

namespace AmpProcessing {
	namespace Lua {

		LuaFile::LuaFile(const std::string& fileName, const std::string& fileContent) : m_LuaContext(luaL_newstate(), lua_close), m_FileName(fileName),
			m_FileContent(fileContent)
		{
			luaL_openlibs(m_LuaContext.get());

			LOG_INFO("[LUA] has been loaded for {}", fileName);
		}

		LuaFile::~LuaFile() {}

		bool LuaFile::Compile()
		{
			LOG_INFO("[LUA] Compiled the file {}", m_FileName);
			int success = luaL_dostring(m_LuaContext.get(), m_FileContent.c_str());
			CheckLua(success);

			return success;
		}

		bool LuaFile::Compile(const std::string& content)
		{
			m_FileContent = content;
			return Compile();
		}

		void LuaFile::CallGlobalFunction(const std::string& functionName) const
		{
			auto* context = m_LuaContext.get();
			lua_getglobal(context, functionName.c_str());

			if (!lua_isfunction(context, -1))
			{
				LOG_WARN("[LUA] {} was not found in file {}", functionName, m_FileName);
				return;
			}

			CheckLua(lua_pcall(context, 0, 0, 0));
		}

		const bool LuaFile::IsFunction(const std::string& functionName) const
		{
			auto* context = m_LuaContext.get();
			lua_getglobal(context, functionName.c_str());
			if (!lua_isfunction(context, -1))
			{
				LOG_WARN("[LUA] {} was not found in file {}", functionName, m_FileName);
				return false;
			}
			lua_pop(context, 1);

			return true;
		}

		bool LuaFile::CheckLua( int r) const
		{
			if (r != LUA_OK)
			{
				std::string errormsg = lua_tostring(m_LuaContext.get(), -1);
				LOG_ERROR("[LUA] {}", errormsg);

				return false;
			}
			return true;
		}

		void LuaFile::PrintStack()
		{
			std::ostringstream ss;
			auto* L = m_LuaContext.get();
			int top = lua_gettop(L);
			if (top == 0)
			{
				LOG_INFO("[LUA Stack] Stack is empty!");
				return;
			}

			for (int i = 1; i <= top; i++) {
				ss << i << "\t" << luaL_typename(L, i) << "\t";
				switch (lua_type(L, i)) {
				case LUA_TNUMBER:
					ss << lua_tonumber(L, i) << "\n";
					break;
				case LUA_TSTRING:
					ss << lua_tostring(L, i) << "\n";
					break;
				case LUA_TBOOLEAN:
					ss << (lua_toboolean(L, i) ? "true" : "false") << "\n";
					break;
				case LUA_TNIL:
					ss << "nil\n";
					break;
				default:
					ss << lua_topointer(L, i) << "\n";
					break;
				}
			}
			LOG_INFO("[LUA Stack] {}", ss.str());
		}
	}
}