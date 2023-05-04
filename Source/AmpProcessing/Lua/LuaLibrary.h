#pragma once

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Lua {
		struct SampleUserdata {
			std::vector<float>* Data;
		};

		class LuaLibrary
		{
		public:
			static void OpenLibs(lua_State* L);

			static SampleUserdata* CreateSampleUserData(lua_State* L);

		private:
			static int SampleNewIndex(lua_State* L);
			static int SampleLength(lua_State* L);
			static void SampleRegister(lua_State* L);
			static const luaL_Reg s_SampleMetaTable[];
			static const char* s_SampleMetaTableName;
		private:
			LuaLibrary() {};
			~LuaLibrary() {};
		};
	}
}