#pragma once

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Lua {

		/// <summary>
		/// Class to handle effect parameter lua library
		/// </summary>
		class EffectParameterLibrary
		{
		public:
			EffectParameterLibrary();
			~EffectParameterLibrary();

			/// <summary>
			/// Registers the required meta tables etc and sets the global variable.
			/// </summary>
			/// <param name="L">- The lua state to register to.</param>
			/// <param name="effectProcessor">- The effect processor class to push to meta table</param>
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