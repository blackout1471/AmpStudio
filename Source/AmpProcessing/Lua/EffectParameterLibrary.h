#pragma once

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}


namespace AmpProcessing {
	namespace Lua {
		class EffectParameterLibrary
		{
		public:
			EffectParameterLibrary();
			~EffectParameterLibrary();

			void OpenLibs(lua_State* L, void* effectProcessor) const;

		private:
			static int NewParameter(lua_State* L);
			static int GetParameterValue(lua_State* L);

		private:
			static const luaL_Reg s_MetaTable[];
			static const std::string s_MetaTableName;
		};
	}
}