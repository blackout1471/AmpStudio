#include "amppch.h"
#include "LuaEffectProcessor.h"
#include "Lua/LuaLibrary.h"

namespace AmpProcessing {
	namespace Effects {

		static bool CheckLua(lua_State* L, int r)
		{
			if (r != LUA_OK)
			{
				std::string errormsg = lua_tostring(L, -1);
				LOG_ERROR("[LUA] {}", errormsg);

				return false;
			}
			return true;
		}

		static int NewParameter(lua_State* L) {
			auto* gClass = (LuaEffectProcessor*)lua_touserdata(L, lua_upvalueindex(1));

			std::string name = lua_tostring(L, 1);
			float max = lua_tonumber(L, 2);
			float min = lua_tonumber(L, 3);
			float step = lua_tonumber(L, 4);
			auto parameter = gClass->AddParameter(Controls::EffectParameter{ name, max, min, step });
			lua_pushlightuserdata(L, parameter.get());

			return 1;
		}

		static int GetParameterValue(lua_State* L) {

			auto* gClass = (Controls::EffectParameter*)lua_touserdata(L, 1);

			auto value = gClass->Value;
			lua_pushnumber(L, value);

			return 1;
		}

		static const struct luaL_Reg parameterLib[] = {
			{"add", NewParameter},
			{"getValue", GetParameterValue},
			{NULL, NULL }
		};

		LuaEffectProcessor::LuaEffectProcessor(const Lua::LuaFile* luaFile) : IEffectProcessor(luaFile->GetFileName()),
			m_LuaFile(luaFile)
		{
			ValidateFile();

			auto* L = m_LuaFile->GetState();

			Lua::LuaLibrary::OpenLibs(L);
			
			luaL_newmetatable(L, "parameter");
			lua_pushlightuserdata(L, this);
			luaL_setfuncs(L, parameterLib, 1);
			lua_setglobal(L, "parameter");

			InitializeEffect();
		}

		LuaEffectProcessor::~LuaEffectProcessor()
		{
		}

		void LuaEffectProcessor::ValidateFile() 
		{
			if (!m_LuaFile->IsFunction(c_OnInitFunctionName))
			{
				LOG_ERROR("[LuaEffectProcessor] {} not exists for {}", c_OnInitFunctionName, m_LuaFile->GetFileName());
			}

			if (!m_LuaFile->IsFunction(c_SampleReadyFunctionName))
			{
				LOG_ERROR("[LuaEffectProcessor] {} not exists for {}", c_SampleReadyFunctionName, m_LuaFile->GetFileName());
			}
		}

		void LuaEffectProcessor::InitializeEffect()
		{

			auto* L = m_LuaFile->GetState();
			
			lua_getglobal(L, c_OnInitFunctionName);

			CheckLua(L, lua_pcall(L, 0, 0, 0));
		}

		void LuaEffectProcessor::Process(std::vector<float>& sample)
		{
			auto* L = m_LuaFile->GetState();

			auto* userdata = Lua::LuaLibrary::CreateSampleUserData(L);
			userdata->Data = &sample;
			
			m_LuaFile->CallLuaFunctionWithArgument(c_SampleReadyFunctionName);

			// remove the userdata object
			lua_pop(L, 1);
		}
	}
}