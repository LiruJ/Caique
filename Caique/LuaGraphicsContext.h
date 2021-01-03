#pragma once

// Graphical includes.
#include <GraphicsContext.h>

// Type Includes.
#include <memory>

// Forward declarations.
namespace Lua { class LuaContext; }
struct lua_State;


namespace LuaGameObjects
{
	constexpr const char* GLOBALGRAPHICSCONTEXTNAME = "graphicsContext";
	constexpr const char* WINDOWWIDTHNAME = "windowWidth";
	constexpr const char* WINDOWHEIGHTNAME = "windowHeight";
	constexpr const char* WINDOWTITLENAME = "windowTitle";
	constexpr const char* WINDOWVSYNCNAME = "vsync";

	class LuaGraphicsContext
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<Graphics::GraphicsContext> graphicsContext);

		static int SetIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int GetIndex(std::shared_ptr<Lua::LuaContext> luaContext);
	};
}