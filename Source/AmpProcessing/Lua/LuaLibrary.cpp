#include "amppch.h"
#include "LuaLibrary.h"

namespace AmpProcessing {
	namespace Lua {
		const char* LuaLibrary::s_SampleMetaTableName = "SampleMetaTable";

		const luaL_Reg LuaLibrary::s_SampleMetaTable[] = {
			{"__len", LuaLibrary::SampleLength},
			{"__newindex", LuaLibrary::SampleNewIndex},
			{nullptr, nullptr}
		};

		void LuaLibrary::OpenLibs(lua_State* L)
		{
			SampleRegister(L);
		}

		SampleUserdata* LuaLibrary::CreateSampleUserData(lua_State* L)
		{
			auto* userdata = static_cast<SampleUserdata*>(lua_newuserdata(L, sizeof(SampleUserdata)));
			luaL_getmetatable(L, s_SampleMetaTableName);
			lua_setmetatable(L, -2);

			return userdata;
		}

		void LuaLibrary::SampleRegister(lua_State* L)
		{
			luaL_newmetatable(L, s_SampleMetaTableName);

			luaL_setfuncs(L, s_SampleMetaTable, 0);

			lua_pop(L, 1);
		}

		int LuaLibrary::SampleLength(lua_State* L)
		{
			auto* userdata = static_cast<SampleUserdata*>(lua_touserdata(L, -1));
			lua_pushinteger(L, userdata->Data->size());
			return 1;
		}


		int LuaLibrary::SampleNewIndex(lua_State* L)
		{
			auto* userdata = static_cast<std::vector<float>*>(lua_touserdata(L, -3));

			int key = lua_tointeger(L, -2);

			float value = lua_tonumber(L, -1);

			(*userdata)[key - 1] = value;

			return 0;
		}
	}
}