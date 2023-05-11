#pragma once
#include "Effects/EffectProcessorMetaData.h"

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Lua {
		class MetaDataLibrary
		{
		public:
			MetaDataLibrary();
			~MetaDataLibrary();
			void OpenLibs(lua_State* L);

			Effects::EffectProcessorMetaData** CreateMetaUserData(lua_State* L, int stackPlace);

		private:
			void RegisterTable(lua_State* L);

			static int MetaTable_NewIndex(lua_State* L);
		private:
			inline static const luaL_Reg s_MetaTable[] = {
				{"__newindex", MetaTable_NewIndex},
				{nullptr, nullptr}
			};
			inline static const char* s_MetaTableName = "MetaDataMetaTable";
		};
	}
}