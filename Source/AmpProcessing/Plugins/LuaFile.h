#pragma once

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Plugins {
		class LuaFile
		{
		public:
			LuaFile(const std::string& fileName, const std::string& fileContent);
			~LuaFile();

			bool Compile();
			bool Compile(const std::string& content);

			void CallLuaFunction(const std::string& functionName);

		private:
			bool CheckLua(lua_State* L, int r);

		private:
			std::unique_ptr<lua_State, decltype(&lua_close)> m_LuaContext;
			std::string m_FileName;
			std::string m_FileContent;
		};
	}
}