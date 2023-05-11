#include "amppch.h"
#include "MetaDataLibrary.h"

#include <sstream>


namespace AmpProcessing {
	namespace Lua {

		MetaDataLibrary::MetaDataLibrary()
		{
		}

		MetaDataLibrary::~MetaDataLibrary()
		{
		}

		void MetaDataLibrary::OpenLibs(lua_State* L)
		{
			RegisterTable(L);
		}

		Effects::EffectProcessorMetaData** MetaDataLibrary::CreateMetaUserData(lua_State* L, int stackPlace)
		{
			auto* userdata = static_cast<Effects::EffectProcessorMetaData**>(lua_newuserdata(L, sizeof(Effects::EffectProcessorMetaData*)));
			luaL_getmetatable(L, s_MetaTableName);
			lua_setmetatable(L, stackPlace);

			return userdata;
		}

		void MetaDataLibrary::RegisterTable(lua_State* L)
		{
			luaL_newmetatable(L, s_MetaTableName);

			luaL_setfuncs(L, s_MetaTable, 0);

			lua_pop(L, 1);
		}

		static void PrintStack(lua_State* L)
		{
			std::ostringstream ss;
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

		int MetaDataLibrary::MetaTable_NewIndex(lua_State* L)
		{
			PrintStack(L);
			auto** userData = static_cast<Effects::EffectProcessorMetaData**>(lua_touserdata(L, 1));

			(*userData)->Name = "Hulla bulla";

			return 0;
		}
	}
}