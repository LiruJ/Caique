#pragma once

// Type Includes.
#include <memory>

// Forward declarations.
namespace Lua { class LuaContext; }
namespace GameTiming { struct GameTime; }

namespace LuaGameObjects
{
	class LuaGameTime
	{
	public:
		static void CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, GameTiming::GameTime& gameTime);
	};
}