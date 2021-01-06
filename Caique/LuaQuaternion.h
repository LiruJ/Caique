#pragma once

// Type Includes.
#include <memory>

// GLM includes.
#include <glm/glm.hpp>

// Forward declarations.
namespace Lua { class LuaContext; }

namespace LuaGameObjects
{
	constexpr const char* QUATERNIONTYPENAME = "quaternion";

	constexpr const char* EULERNAME = "euler";
	constexpr const char* QUATNORMALNAME = "normal";


	class LuaQuaternion
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext);

		static void CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, glm::quat quaternion);
	private:
		static int getIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int setIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int toString(std::shared_ptr<Lua::LuaContext> luaContext);

		static int createNewFromXYZW(std::shared_ptr<Lua::LuaContext> luaContext);

		static int createNewFromEuler(std::shared_ptr<Lua::LuaContext> luaContext);

		static int createNewFromAngleAxis(std::shared_ptr<Lua::LuaContext> luaContext);

		static int multiply(std::shared_ptr<Lua::LuaContext> luaContext);
	};
}