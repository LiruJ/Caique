#pragma once

// Type includes.
#include <memory>
#include <string>
#include <map>

#include "LuaContext.h"

namespace Lua
{
	constexpr const char* SCRIPTINSTANCENAME = "scriptInstance";
	constexpr const char* FUNCTIONTABLENAME = "instanceFunctions";
	constexpr const char* ENVIRONMENTSTABLENAME = "instanceEnvironments";

	// Represents a Lua file with its own environment table which defaults to the global table.
	class LuaScript
	{
	public:
		~LuaScript();

		static int SaveInstanceFunction(std::shared_ptr<Lua::LuaContext> luaContext, const std::string& luaSource);

		static std::shared_ptr<LuaScript> CreateInstance(std::shared_ptr<Lua::LuaContext> luaContext, const int functionID);

		static void Register(std::shared_ptr<Lua::LuaContext> luaContext);

		void Setup();

		bool HasFunction(const std::string& name);

		bool RunFunctionUsingStack(const std::string& name, int argumentCount);

		bool RunFunction(const std::string& name);

		template<typename ... Args>
		void RunFunction(const std::string& name, Args&&... args)
		{
			// Start balancing.
			luaContext->BeginBalancing();

			// Retrieve the function.
			int function = GetEnvironmentField(name);

			// If the field is a function, call it.
			if (luaContext->IsFunction(function))
			{
				// Get the number of given arguments.
				int argumentCount = sizeof...(Args);

				// Push the values to the stack.
				luaContext->Push(args...);

				if (!luaContext->CallFunction(argumentCount, 0)) throw std::exception("Failed to run lua script.");
			}
			// Otherwise; remove the function from the stack.
			else luaContext->Remove(function);

			// Stop balancing.
			luaContext->StopBalancing();
		}

		int GetEnvironment();

		int GetEnvironmentField(const std::string& name);
		void SetEnvironmentField(const std::string& name, const int stackIndex);

		std::shared_ptr<Lua::LuaContext> GetLuaContext() { return luaContext; }

	private:
		std::shared_ptr<Lua::LuaContext> luaContext;
		std::map<std::string, bool> cachedFunctionQueries;

		LuaScript(std::shared_ptr<LuaContext> luaContext , int environmentID, int functionID) : luaContext(luaContext), environmentID(environmentID), functionID(functionID) { }

		// The ID of the Lua code (anonymous function) that this script uses.
		int functionID;

		// The ID of this script's table within the registry.
		int environmentID;
	};
}