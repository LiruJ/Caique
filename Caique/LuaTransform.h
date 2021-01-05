#pragma once

// Type Includes.
#include <memory>

// Forward declarations.
namespace Lua { class LuaContext; }
namespace GameObjects { class Transform; }

namespace LuaGameObjects
{
	constexpr const char* TRANSFORMTYPENAME = "transform";

	constexpr const char* LOCALPOSITIONNAME = "localPosition";
	constexpr const char* LOCALROTATIONNAME = "localRotation";

	class LuaTransform
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext);

		static void CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameObjects::Transform> transform);
	private:
		static int getIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int setIndex(std::shared_ptr<Lua::LuaContext> luaContext);
	};
}