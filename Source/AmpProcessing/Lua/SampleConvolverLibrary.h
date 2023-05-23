#pragma once
#include "DSP/Convolution/ISampleConvolver.h"

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Lua {
		class SampleConvolverLibrary
		{
		public:
			SampleConvolverLibrary();
			~SampleConvolverLibrary();

			bool OpenLibs(lua_State* L);

		private:
			DSP::ISampleConvolver* CreateConvolver();

			static int NewConvolver(lua_State* L);
			static int ProcessConvolver(lua_State* L);

		private:
			inline static const luaL_Reg s_ConvolverMetaTable[] = {
				{nullptr, nullptr}
			};
			inline static const char* s_ConvolverMetaTableName = "convolverMetaTable";

			std::vector<std::unique_ptr<DSP::ISampleConvolver>> m_Convolvers;
		};
	}
}
