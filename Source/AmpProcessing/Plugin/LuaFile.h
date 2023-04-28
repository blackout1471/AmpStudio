#pragma once

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Plugin {
		class LuaFile
		{
		public:
			LuaFile(const std::string& path, const std::string& fileContent);
			~LuaFile();

			bool Compile();

		private:
			bool CheckLua(lua_State* L, int r);

		private:
			std::unique_ptr<lua_State> m_LuaContext;
			std::string m_FilePath;
			std::string m_FileContent;
		};
	}
}