#include "amppch.h"
#include "LuaEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {
		LuaEffectProcessor::LuaEffectProcessor(const Utility::File& file) : IEffectProcessor(file.GetFileName()), 
			m_LuaFile(Plugins::LuaFile(file.GetFileName(), file.ReadContent()))
		{
			m_LuaFile.Compile();
		}

		LuaEffectProcessor::~LuaEffectProcessor()
		{
		}

		void LuaEffectProcessor::Process(std::vector<float>& sample)
		{
			m_LuaFile.CallLuaFunction("OnInit");
		}
	}
}