#include "amppch.h"
#include "LuaEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {
		LuaEffectProcessor::LuaEffectProcessor(const Plugins::LuaFile* luaFile) : IEffectProcessor(luaFile->GetFileName()),
			m_LuaFile(luaFile)
		{
		}

		LuaEffectProcessor::~LuaEffectProcessor()
		{
		}

		void LuaEffectProcessor::Process(std::vector<float>& sample)
		{
			m_LuaFile->CallLuaFunction("OnInit");
		}
	}
}