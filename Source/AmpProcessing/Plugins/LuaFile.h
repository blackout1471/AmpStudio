#pragma once

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Plugins {
		class LuaFile
		{
		public:
			LuaFile(const std::string& fileName, const std::string& fileContent);
			~LuaFile();

			bool Compile();
			bool Compile(const std::string& content);

			void CallLuaFunction(const std::string& functionName) const;
			const bool IsFunction(const std::string& functionName) const;

			inline const std::string& GetFileName() const { return m_FileName; }

			// TODO:: remove context and re-write methods to not be dependent,
			// This is only for developing phase.
			inline lua_State* GetState() const { return m_LuaContext.get(); }; 

		private:
			bool CheckLua(lua_State* L, int r) const;

		private:
			std::unique_ptr<lua_State, decltype(&lua_close)> m_LuaContext;
			std::string m_FileName;
			std::string m_FileContent;
		};
	}
}