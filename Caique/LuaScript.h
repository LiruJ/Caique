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

		/// <summary> Loads the given source string and saves the resulting function to the registry, returning the index. </summary>
		/// <param name="luaContext"> The lua context to use. </param>
		/// <param name="luaSource"> The lua string source. </param>
		/// <returns> The index of the function within the table on the registry. </returns>
		static int SaveInstanceFunction(std::shared_ptr<Lua::LuaContext> luaContext, const std::string& luaSource);

		/// <summary> Creates and returns an instance of the function at the given ID with a fresh environment table. </summary>
		/// <param name="luaContext"> The lua context to use. </param>
		/// <param name="functionID"> The ID of the function within the table on the registry, returned by SaveInstanceFunction(). </param>
		/// <returns> The created lua script. </returns>
		static std::shared_ptr<LuaScript> CreateInstance(std::shared_ptr<Lua::LuaContext> luaContext, const int functionID);

		/// <summary> Registers the metatable and any other information for this object. </summary>
		/// <param name="luaContext"> The lua context to use. </param>
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext);

		/// <summary> Runs the function, setting values in this script's environment table. </summary>
		void Setup();

		/// <summary> Finds if this script defines a function with the given name. </summary>
		/// <param name="name"> The name of the function. </param>
		/// <returns> True if the function exists; otherwise false. </returns>
		bool HasFunction(const std::string& name);

		/// <summary> Runs the function with the given name using the existing values on the stack. </summary>
		/// <param name="name"> The name of the function. </param>
		/// <param name="argumentCount"> The number of values placed onto the stack before this function was called. </param>
		/// <returns> True if the function was successfully run; otherwise false. </returns>
		bool RunFunctionUsingStack(const std::string& name, int argumentCount);

		/// <summary> Runs the functon with the given name, taking no arguments. </summary>
		/// <param name="name"> The name of the function. </param>
		/// <returns> True if the function was successfully run; otherwise false. </returns>
		bool RunFunction(const std::string& name);

		/// <summary> Runs the function with the given name using the given arguments. </summary>
		/// <typeparam name="...Args"> The arguments to pass to the function. </typeparam>
		/// <param name="name"> The name of the function. </param>
		/// <param name="...args"> The arguments to pass to the function. </param>
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

		/// <summary> Pushes this script's environment table onto the stack and returns its stack index. </summary>
		/// <returns> The stack index of the table. </returns>
		int GetEnvironment();

		/// <summary> Gets the field from this script's environment table with the given name and pushes it onto the stack, removing the table. </summary>
		/// <param name="name"> The name of the field. </param>
		/// <returns> The stack index of the value. </returns>
		int GetEnvironmentField(const std::string& name);

		/// <summary> Sets the field in this script's environment table with the given name to the item on the stack with the given index. </summary>
		/// <param name="name"> The name of the field. </param>
		/// <param name="stackIndex"> The stack index of the value to set. </param>
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