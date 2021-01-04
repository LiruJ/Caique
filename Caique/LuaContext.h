#pragma once

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "../Libraries/Lua/lua51.lib")
#endif

#define method(class, name) {#name, class::name}

// Type includes.
#include <memory>
#include <string>
#include <map>

#include <stdarg.h>

namespace Lua
{
	class LuaContext;
	typedef int (*luaContextFunction) (std::shared_ptr<LuaContext> luaContext);

	constexpr int OK = 0;

	constexpr const char* GLOBALTABLENAME = "_G";
	constexpr const char* INDEXERNAME = "__index";
	constexpr const char* NEWINDEXERNAME = "__newindex";
	constexpr const char* TOSTRINGNAME = "__tostring";
	constexpr const char* ADDNAME = "__add";
	constexpr const char* SUBTRACTNAME = "__sub";
	constexpr const char* MULTIPLYNAME = "__mul";
	constexpr const char* DIVIDENAME = "__div";

	constexpr const int GLOBALSINDEX = LUA_GLOBALSINDEX;
	constexpr const int REGISTRYINDEX = LUA_REGISTRYINDEX;

	constexpr const int INVALIDREFERENCE = LUA_NOREF;
	constexpr const int NILREFERENCE = LUA_REFNIL;
	constexpr const int INVALIDSTACKCOUNT = -10000;

	class LuaContext : public std::enable_shared_from_this<Lua::LuaContext>
	{
	public:
		~LuaContext();

		int GetTopIndex();

		void BeginBalancing();
		void StopBalancing();

		int GetGlobal(const std::string& name);
		void SetGlobal(const std::string& name);

		bool LoadFile(const std::string& filePath);
		bool LoadString(const std::string& source);

		template<typename... Args>
		int Error(const std::string& formattedMessage, Args... args)
		{
			return luaL_error(luaState, formattedMessage.c_str(), args...);
		}

		template<typename... Args>
		void Push(int value, Args&&... args)
		{
			Push(value);
			Push(args...);
		}

		template<typename... Args>
		void Push(double value, Args&&... args)
		{
			Push(value);
			Push(args...);
		}

		template<typename... Args>
		void Push(const std::string& value, Args&&... args)
		{
			Push(value);
			Push(args...);
		}

		template<typename... Args>
		void Push(const luaContextFunction value, Args&&... args)
		{
			Push(value);
			Push(args...);
		}

		void Push(const std::string& value);
		void Push(const int value);
		void Push(const double value);
		void Push(const luaContextFunction value);

		int PushNil();
		int PushReadOnlyErrorFunction();

		int PushNewTable();
		
		void* PushUserData(const int size);

		int PushNewMetaTable(const std::string& typeName);
		void SetMetaTable(const int stackIndex);
		int GetMetaTable(const std::string& typeName);

		void CopyFrom(const int stackIndex);

		int MoveTop(const int stackIndex);

		void Remove(const int stackIndex);

		bool SetUpValue(const int stackIndex, const int upValueIndex, const char*& upValueName);

		void Register(const std::string& libraryName, const luaL_Reg* methods);
		void Register(const luaL_Reg* methods);

		int GetField(const std::string& name, const int stackIndex);
		void SetField(const std::string& name, const int stackIndex);

		void GetEnv(const int stackIndex);
		bool SetEnv(const int stackIndex);

		std::string GetType(const int stackIndex);

		int AddTo(const int stackIndex);
		bool AddToRegistry(const std::string& name);
		void RetrieveFrom(const int stackIndex, const int tableIndex);
		bool RetrieveFromRegistry(const std::string& name);
		void RemoveFrom(const int stackIndex, const int tableIndex);
		bool RemoveFromRegistry(const std::string& name);

		bool IsNil(const int stackIndex);
		bool IsTable(const int stackIndex);
		bool IsUserData(const int stackIndex);
		bool IsFunction(const int stackIndex);
		bool IsDouble(const int stackIndex);

		void* ToUserData(const int stackIndex);
		std::string ToString(const int stackIndex);
		int ToInt(const int stackIndex);
		double ToDouble(const int stackIndex);
		float ToFloat(const int stackIndex) { return (float)ToDouble(stackIndex); }

		void* CheckUserData(const int stackIndex, const std::string& typeName);
		std::string CheckString(const int stackIndex);
		int CheckInt(const int stackIndex);
		double CheckDouble(const int stackIndex);
		float CheckFloat(const int stackIndex) { return (float)CheckDouble(stackIndex); }

		bool CallFunction(const int argumentCount, const int returnValueCount);

		static std::shared_ptr<LuaContext> Create();
	private:
		lua_State* luaState;

		int balanceIndex;

		std::map<std::string, int> namesToRegistryIndices;

		static int interceptFunctionCall(lua_State* calledState);

		static int readOnlyErrorFunction(std::shared_ptr<Lua::LuaContext> luaContext);

		LuaContext();

		bool validateLua(int returnValue);
	};
}