#include "amppch.h"
#include "LuaEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {


		LuaEffectProcessor::LuaEffectProcessor(const Lua::LuaFile* luaFile) : IEffectProcessor(luaFile->GetFileName()),
			m_LuaFile(luaFile), m_EffectParameterLibrary(), m_SampleLibrary(), m_MetaDataLibrary()
		{
			ValidateFile();

			auto* L = m_LuaFile->GetState();

			m_SampleLibrary.OpenLibs(L);
			m_EffectParameterLibrary.OpenLibs(L, this);
			m_MetaDataLibrary.OpenLibs(L);

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

			auto** metaUserData = m_MetaDataLibrary.CreateMetaUserData(L, -2);
			*metaUserData = &GetMetaData();

			lua_getglobal(L, c_OnInitFunctionName);
			lua_pushvalue(L, -2);
			m_LuaFile->CheckLua(lua_pcall(L, 1, 0, 0));

			lua_pop(L, 1);
		}

		void LuaEffectProcessor::Process(std::vector<float>& sample)
		{
			auto* L = m_LuaFile->GetState();

			auto* userdata = m_SampleLibrary.CreateSampleUserData(L, -2);
			userdata->Data = &sample;

			lua_getglobal(L, c_SampleReadyFunctionName);

			lua_pushvalue(L, -2);

			m_LuaFile->CheckLua(lua_pcall(L, 1, 0, 0));

			lua_pop(L, 1);
		}
	}
}