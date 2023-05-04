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


		LuaEffectProcessor::LuaEffectProcessor(const Lua::LuaFile* luaFile) : IEffectProcessor(luaFile->GetFileName()),
			m_LuaFile(luaFile)
		{
			ValidateFile();

			auto* L = m_LuaFile->GetState();

			Lua::LuaLibrary::OpenLibs(L);
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
			m_LuaFile->CallLuaFunction(c_OnInitFunctionName);
		}

		void LuaEffectProcessor::Process(std::vector<float>& sample)
		{
			auto* L = m_LuaFile->GetState();

			auto* userdata = Lua::LuaLibrary::CreateSampleUserData(L);
			userdata = &sample;

			// push the Lua function onto the stack
			lua_getglobal(L, c_SampleReadyFunctionName);

			// push the userdata object onto the stack as an argument to the function
			lua_pushvalue(L, -2);

			// call the function with 1 argument and 0 return values
			CheckLua(L, lua_pcall(L, 1, 0, 0));

			// remove the userdata object and function from the stack
			lua_pop(L, 1);
		}

		
	}
}