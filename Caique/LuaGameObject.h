#pragma once

// Type Includes.
#include <memory>

// Forward declarations.
namespace Lua { class LuaContext; }
namespace GameObjects { class GameObject; }

namespace LuaGameObjects
{
	constexpr const char* GAMEOBJECTTYPENAME = "gameObject";

	constexpr const char* ADDCOMPONENTNAME = "addComponent";
	constexpr const char* ADDGAMEOBJECTNAME = "addNewGameObject";

	class LuaGameObject
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext);

		static void CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameObjects::GameObject> gameObject);
	private:
		static int getIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int addComponent(std::shared_ptr<Lua::LuaContext> luaContext);

		static int addNewGameObject(std::shared_ptr<Lua::LuaContext> luaContext);
	};
}