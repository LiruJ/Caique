#pragma once

// Type Includes.
#include <memory>

// Forward declarations.
namespace Lua { class LuaContext; }
namespace Input { class InputManager; }

namespace LuaGameObjects
{
	constexpr const char* GLOBALINPUTMANAGERNAME = "inputManager";

	constexpr const char* GETKEYDOWNNAME = "getKeyDown";
	constexpr const char* GETKEYUPNAME = "getKeyUp";
	constexpr const char* ISKEYDOWNNAME = "isKeyDown";
	constexpr const char* ISKEYUPNAME = "isKeyUp";
	constexpr const char* WASKEYDOWNNAME = "wasKeyDown";
	constexpr const char* WASKEYUPNAME = "wasKeyUp";

	constexpr const char* MOUSEDELTAXNAME = "mouseDeltaX";
	constexpr const char* MOUSEDELTAYNAME = "mouseDeltaY";

	class LuaInputManager
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<Input::InputManager> inputManager);
	private:
		static int indexFunction(std::shared_ptr<Lua::LuaContext> luaContext);

		static int getKeyDown(std::shared_ptr<Lua::LuaContext> luaContext);
		static int getKeyUp(std::shared_ptr<Lua::LuaContext> luaContext);
		static int isKeyDown(std::shared_ptr<Lua::LuaContext> luaContext);
		static int isKeyUp(std::shared_ptr<Lua::LuaContext> luaContext);
		static int wasKeyDown(std::shared_ptr<Lua::LuaContext> luaContext);
		static int wasKeyUp(std::shared_ptr<Lua::LuaContext> luaContext);

		static void getFunctionArguments(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<Input::InputManager>& inputManager, const char*& keyName);
	};
}