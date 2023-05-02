#pragma once
#include "IEffectProcessor.h"
#include "Utility/File.h"
#include "Plugins/LuaFile.h"

namespace AmpProcessing {
	namespace Effects {
		class LuaEffectProcessor : public IEffectProcessor
		{
		public:
			LuaEffectProcessor(const Plugins::LuaFile* luaFile);
			~LuaEffectProcessor();

			virtual void Process(std::vector<float>& sample) override;

		private:
			void ValidateFile();
		private:
			const Plugins::LuaFile* m_LuaFile;
		};
	}
}