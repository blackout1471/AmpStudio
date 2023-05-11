#pragma once

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

namespace AmpProcessing {
	namespace Lua {
		class LuaFile
		{
		public:
			LuaFile(const std::string& fileName, const std::string& fileContent);
			~LuaFile();

			/// <summary>
			/// Compiles the already given content of the file.
			/// </summary>
			/// <returns>Whether the compile was successfull.</returns>
			bool Compile();

			/// <summary>
			/// Compiles the newly given content and assigns it.
			/// </summary>
			/// <param name="content">- The new content to compile.</param>
			/// <returns>Whether the compile was successfull.</returns>
			bool Compile(const std::string& content);

			/// <summary>
			/// Calls a global function in the lua file.
			/// 0 arguments, 0 return values.
			/// </summary>
			/// <param name="functionName">- The name of the function to call.</param>
			void CallGlobalFunction(const std::string& functionName) const;

			/// <summary>
			/// Checks whether the given function name is an function in the lua file.
			/// </summary>
			/// <param name="functionName">- The function name to check.</param>
			/// <returns>Whether the function name is a function</returns>
			const bool IsFunction(const std::string& functionName) const;

			/// <returns>The name of the file</returns>
			inline const std::string& GetFileName() const { return m_FileName; }

			// TODO:: remove context and re-write methods to not be dependent,
			// This is only for developing phase.
			inline lua_State* GetState() const { return m_LuaContext.get(); }; 

			/// <summary>
			/// Checks whether the given lua code is valid.
			/// </summary>
			/// <param name="r">- The return code to check</param>
			/// <returns>True if valid, false if not.</returns>
			bool CheckLua(int r) const;

			/// <summary>
			/// Prints the current lua stack.
			/// </summary>
			void PrintStack() const;

		private:
			std::unique_ptr<lua_State, decltype(&lua_close)> m_LuaContext;
			std::string m_FileName;
			std::string m_FileContent;
		};
	}
}