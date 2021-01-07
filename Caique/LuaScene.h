#pragma once

// Type Includes.
#include <memory>

// Forward declarations.
namespace Lua { class LuaContext; }
namespace GameObjects { class Scene; }

namespace LuaGameObjects
{
	constexpr const char* SCENETYPENAME = "scene";

	constexpr const char* ADDSCENEGAMEOBJECTNAME = "addNewGameObject";


	class LuaScene
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameObjects::Scene> scene);
	private:
		static int getIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int addNewGameObject(std::shared_ptr<Lua::LuaContext> luaContext);
	};
}