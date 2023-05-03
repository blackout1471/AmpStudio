#pragma once
#include "IEffectProcessor.h"
#include "Utility/File.h"
#include "Lua/LuaFile.h"

namespace AmpProcessing {
	namespace Effects {
		class LuaEffectProcessor : public IEffectProcessor
		{
		public:
			LuaEffectProcessor(const Lua::LuaFile* luaFile);
			~LuaEffectProcessor();

			virtual void Process(std::vector<float>& sample) override;

		private:
			void ValidateFile();

			void InitializeEffect();

		private:
			const Lua::LuaFile* m_LuaFile;
		private:
			const char* c_OnInitFunctionName = "OnInit";
			const char* c_SampleReadyFunctionName = "OnReadySample";
		};
	}
}