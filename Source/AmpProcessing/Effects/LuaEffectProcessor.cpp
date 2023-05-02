#include "amppch.h"
#include "LuaEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {
		LuaEffectProcessor::LuaEffectProcessor(const Plugins::LuaFile* luaFile) : IEffectProcessor(luaFile->GetFileName()),
			m_LuaFile(luaFile)
		{
			ValidateFile();
		}

		LuaEffectProcessor::~LuaEffectProcessor()
		{
		}

		void LuaEffectProcessor::ValidateFile() 
		{
			if (!m_LuaFile->IsFunction("OnInit"))
				LOG_ERROR("[LuaEffectProcessor] OnInit not exists for {}", m_LuaFile->GetFileName());
		}

		void LuaEffectProcessor::Process(std::vector<float>& sample)
		{
			m_LuaFile->CallLuaFunction("OnInit");
		}
	}
}