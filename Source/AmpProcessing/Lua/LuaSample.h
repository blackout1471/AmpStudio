	#pragma once
#include <vector>
#include <functional>

extern "C" {
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
}

namespace AmpProcessing {
	namespace Lua {
		struct SampleData {
			std::vector<float>* data;
		};

		static int SampleLength(lua_State* L) {
			auto* userdata = static_cast<SampleData*>(lua_touserdata(L, -1));
			lua_pushinteger(L, userdata->data->size());
			return 1;
		}

		static int SampleNewIndex(lua_State* L) {
			// get the userdata object
			SampleData* userdata = static_cast<SampleData*>(lua_touserdata(L, -3));
			// get the index key
			int key = lua_tointeger(L, -2);
			// get the new value
			float value = lua_tonumber(L, -1);
			// set the value in the vector
			(*userdata->data)[key - 1] = value;

			return 0;
		}

		static const char* s_SampleMetaName = "sampleMetaClass";
		static const luaL_Reg s_SampleMetaData[] = {
			//{"__index", nullptr},
			{"__newindex", SampleNewIndex},
			{"__len", SampleLength},
			{nullptr, nullptr}
		};

		static void SampleRegister(lua_State* L) {
			// create the metatable for the sample userdata object
			luaL_newmetatable(L, s_SampleMetaName);
			// register the metamethods for the metatable
			luaL_setfuncs(L, s_SampleMetaData, 0);

			lua_pop(L, 1);
		}
	}
}