#include "amppch.h"
#include "SampleLibrary.h"

namespace AmpProcessing {
	namespace Lua {
		const char* SampleLibrary::s_SampleMetaTableName = "SampleMetaTable";

		const luaL_Reg SampleLibrary::s_SampleMetaTable[] = {
			{"__len", SampleLibrary::SampleLength},
			{"__newindex", SampleLibrary::SampleNewIndex},
			{"__index", SampleLibrary::SampleIndex},
			{nullptr, nullptr}
		};

		void SampleLibrary::OpenLibs(lua_State* L)
		{
			SampleRegister(L);
		}

		SampleUserdata* SampleLibrary::CreateSampleUserData(lua_State* L, int stackPlace)
		{
			auto* userdata = static_cast<SampleUserdata*>(lua_newuserdata(L, sizeof(SampleUserdata)));
			luaL_getmetatable(L, s_SampleMetaTableName);
			lua_setmetatable(L, stackPlace);

			return userdata;
		}

		void SampleLibrary::SampleRegister(lua_State* L)
		{
			luaL_newmetatable(L, s_SampleMetaTableName);

			luaL_setfuncs(L, s_SampleMetaTable, 0);

			lua_pop(L, 1);
		}

		int SampleLibrary::SampleLength(lua_State* L)
		{
			auto* userdata = static_cast<SampleUserdata*>(lua_touserdata(L, -1));
			lua_pushinteger(L, userdata->Data->size());
			return 1;
		}


		int SampleLibrary::SampleNewIndex(lua_State* L)
		{
			auto* userdata = static_cast<SampleUserdata*>(lua_touserdata(L, -3));

			int key = lua_tointeger(L, -2);

			float value = lua_tonumber(L, -1);

			(*userdata->Data)[key - 1] = value;

			return 0;
		}

		int SampleLibrary::SampleIndex(lua_State* L)
		{
			auto* userdata = static_cast<SampleUserdata*>(lua_touserdata(L, -2));
			int key = lua_tointeger(L, -1);

			lua_pushnumber(L, (*userdata->Data)[key - 1]);

			return 1;
		}
	}
}