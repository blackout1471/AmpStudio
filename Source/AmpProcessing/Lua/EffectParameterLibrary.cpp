#include "amppch.h"
#include "EffectParameterLibrary.h"

#include "Effects/LuaEffectProcessor.h"

namespace AmpProcessing {
	namespace Lua {

		const std::string EffectParameterLibrary::s_MetaTableName = "parameter";

		const luaL_Reg EffectParameterLibrary::s_MetaTable[] = {
			{"add", NewParameter},
			{"getValue", GetParameterValue},
			{nullptr, nullptr}
		};

		EffectParameterLibrary::EffectParameterLibrary()
		{
		}

		EffectParameterLibrary::~EffectParameterLibrary()
		{
		}

		void EffectParameterLibrary::OpenLibs(lua_State* L, void* effectProcessor) const
		{
			luaL_newmetatable(L, "parameter");
			lua_pushlightuserdata(L, effectProcessor);
			luaL_setfuncs(L, s_MetaTable, 1);
			lua_setglobal(L, "parameter");
		}

		int EffectParameterLibrary::NewParameter(lua_State* L) {
			auto* gClass = static_cast<Effects::LuaEffectProcessor*>(lua_touserdata(L, lua_upvalueindex(1)));

			std::string name = lua_tostring(L, 1);
			float max = lua_tonumber(L, 2);
			float min = lua_tonumber(L, 3);
			float step = lua_tonumber(L, 4);
			auto parameter = gClass->AddParameter(Controls::EffectParameter{ name, max, min, step });
			lua_pushlightuserdata(L, parameter.get());

			return 1;
		}

		int EffectParameterLibrary::GetParameterValue(lua_State* L) {

			auto* gClass = static_cast<Controls::EffectParameter*>(lua_touserdata(L, 1));

			auto value = gClass->Value;
			lua_pushnumber(L, value);

			return 1;
		}

		
	}
}