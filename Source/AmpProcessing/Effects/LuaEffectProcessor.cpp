#include "amppch.h"
#include "LuaEffectProcessor.h"

namespace AmpProcessing {
	namespace Effects {

		static bool CheckLua(lua_State* L, int r)
		{
			if (r != LUA_OK)
			{
				std::string errormsg = lua_tostring(L, -1);
				LOG_ERROR("[LUA] {}", errormsg);

				return false;
			}
			return true;
		}

		static void PrintStack(lua_State* L) {
			int top = lua_gettop(L);

			std::string str = "From top to bottom, the lua stack is \n";
			for (unsigned index = top; index > 0; index--)
			{
				int type = lua_type(L, index);
				switch (type)
				{
					// booleans
				case LUA_TBOOLEAN:
					str = str + (lua_toboolean(L, index) ? "true" : "false") + "\n";
					break;

					// numbers
				case LUA_TNUMBER:
					str = str + std::to_string(lua_tonumber(L, index)) + "\n";
					break;

					// strings
				case LUA_TSTRING:
					str = str + lua_tostring(L, index) + "\n";
					break;

					// other
				default:
					str = str + lua_typename(L, type) + "\n";
					break;
				}
			}

			str = str + "\n";
			LOG_INFO(str);
		}

		const char* kSampleMetadataTableName = "sample_userdata_metatable";

		struct SampleData {
			std::vector<float>* data;
		};

		static int SampleLength(lua_State* state) {
			auto* userdata = static_cast<SampleData*>(lua_touserdata(state, -1));
			lua_pushinteger(state, userdata->data->size());
			return 1;
		}

		static const luaL_Reg kSampleMetadataTable[] = {
			//{"__index", nullptr},
			//{"__newindex", nullptr},
			{"__len", SampleLength},
			{nullptr, nullptr}
		};


		LuaEffectProcessor::LuaEffectProcessor(const Plugins::LuaFile* luaFile) : IEffectProcessor(luaFile->GetFileName()),
			m_LuaFile(luaFile)
		{
			ValidateFile();

			auto* L = m_LuaFile->GetState();

			// create the metatable for the sample userdata object
			luaL_newmetatable(L, kSampleMetadataTableName);
			// register the metamethods for the metatable
			luaL_setfuncs(L, kSampleMetadataTable, 0);

			lua_pop(L, 1);
		}

		LuaEffectProcessor::~LuaEffectProcessor()
		{
		}

		void LuaEffectProcessor::ValidateFile() 
		{
			if (!m_LuaFile->IsFunction(c_OnInitFunctionName))
			{

				LOG_ERROR("[LuaEffectProcessor] {} not exists for {}", c_OnInitFunctionName, m_LuaFile->GetFileName());
			}

			if (!m_LuaFile->IsFunction(c_SampleReadyFunctionName))
			{

				LOG_ERROR("[LuaEffectProcessor] {} not exists for {}", c_SampleReadyFunctionName, m_LuaFile->GetFileName());
			}
		}

		void LuaEffectProcessor::InitializeEffect()
		{
			m_LuaFile->CallLuaFunction(c_OnInitFunctionName);
		}

		void LuaEffectProcessor::Process(std::vector<float>& sample)
		{
			auto* L = m_LuaFile->GetState();

			// create a new userdata object that references the sample vector
			auto* userdata = static_cast<SampleData*>(lua_newuserdata(L, sizeof(SampleData)));
			userdata->data = &sample;

			// set the metatable for the userdata object
			luaL_getmetatable(L, kSampleMetadataTableName);
			lua_setmetatable(L, -2);

			// push the Lua function onto the stack
			lua_getglobal(L, c_SampleReadyFunctionName);
			// push the userdata object onto the stack as an argument to the function
			lua_pushvalue(L, -2);

			// call the function with 1 argument and 0 return values
			CheckLua(L, lua_pcall(L, 1, 0, 0));

			// remove the userdata object and function from the stack
			lua_pop(L, 1);
		}

		
	}
}