#pragma once

// Type Includes.
#include <memory>

// GLM includes.
#include <glm/glm.hpp>

// Forward declarations.
namespace Lua { class LuaContext; }

namespace LuaGameObjects
{
	constexpr const char* VECTOR3TYPENAME = "vector3";
	constexpr const char* LENGTHNAME = "length";
	constexpr const char* DISTANCENAME = "distance";
	constexpr const char* DOTNAME = "dot";
	constexpr const char* VECNORMALNAME = "normal";
	constexpr const char* NORMALISENAME = "normalise";
	constexpr const char* NORMALIZENAME = "normalize";

	constexpr const char* UPNAME = "up";
	constexpr const char* DOWNNAME = "down";
	constexpr const char* LEFTNAME = "left";
	constexpr const char* RIGHTNAME = "right";
	constexpr const char* FORWARDNAME = "forward";
	constexpr const char* BACKWARDNAME = "backward";

	class LuaVector3
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext);

		static void CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, glm::vec3 vector3);
	private:
		static int getIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int setIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int toString(std::shared_ptr<Lua::LuaContext> luaContext);

		static int createNew(std::shared_ptr<Lua::LuaContext> luaContext);

		static int add(std::shared_ptr<Lua::LuaContext> luaContext);

		static int subtract(std::shared_ptr<Lua::LuaContext> luaContext);

		static int multiply(std::shared_ptr<Lua::LuaContext> luaContext);
		
		static int divide(std::shared_ptr<Lua::LuaContext> luaContext);

		static int length(std::shared_ptr<Lua::LuaContext> luaContext);

		static int distance(std::shared_ptr<Lua::LuaContext> luaContext);

		static int unarySubtract(std::shared_ptr<Lua::LuaContext> luaContext);

		static int dot(std::shared_ptr<Lua::LuaContext> luaContext);
		
		static int normalise(std::shared_ptr<Lua::LuaContext> luaContext);
	};
}