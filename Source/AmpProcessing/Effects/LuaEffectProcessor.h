#pragma once
#include "IEffectProcessor.h"
#include "Utility/File.h"
#include "Plugins/LuaFile.h"

namespace AmpProcessing {
	namespace Effects {
		class LuaEffectProcessor : public IEffectProcessor
		{
		public:
			LuaEffectProcessor(const Utility::File& file);
			~LuaEffectProcessor();

			virtual void Process(std::vector<float>& sample) override;

		private:
			Plugins::LuaFile m_LuaFile;
		};
	}
}