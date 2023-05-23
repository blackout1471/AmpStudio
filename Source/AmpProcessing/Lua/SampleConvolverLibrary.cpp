#include "amppch.h"
#include "SampleConvolverLibrary.h"
#include "DSP/Convolution/FFTSampleConvolver.h"

namespace AmpProcessing {
	namespace Lua {

		SampleConvolverLibrary::SampleConvolverLibrary()
		{
		}

		SampleConvolverLibrary::~SampleConvolverLibrary()
		{
		}

		bool SampleConvolverLibrary::OpenLibs(lua_State* L)
		{
			luaL_newmetatable(L, s_ConvolverMetaTableName);

			luaL_setfuncs(L, s_ConvolverMetaTable, 0);

			lua_setglobal(L, s_ConvolverMetaTableName);

			return true;
		}

		DSP::ISampleConvolver* SampleConvolverLibrary::CreateConvolver()
		{
			return nullptr;
		}

		int SampleConvolverLibrary::NewConvolver(lua_State* L)
		{
			return 0;
		}

		int SampleConvolverLibrary::ProcessConvolver(lua_State* L)
		{
			return 0;
		}


	}
}