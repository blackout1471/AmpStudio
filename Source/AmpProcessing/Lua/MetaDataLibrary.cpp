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

		int MetaDataLibrary::MetaTable_NewIndex(lua_State* L)
		{
			auto** userData = static_cast<Effects::EffectProcessorMetaData**>(lua_touserdata(L, 1));
			auto caller = std::string(luaL_checkstring(L, 2));
			if (caller == "name")
			{
				(*userData)->Name = luaL_checkstring(L, 3);
				return 0;
			}

			if (caller == "category")
			{
				(*userData)->Category = static_cast<Effects::EffectCategory>((int)luaL_checknumber(L, 3));
				return 0;
			}

			LOG_ERROR("[LUA] '{}' is not an valid property for meta table", caller);

			return 0;
		}
	}
}