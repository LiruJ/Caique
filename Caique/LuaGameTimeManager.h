#pragma once

// Type Includes.
#include <memory>

// Forward declarations.
namespace Lua { class LuaContext; }
namespace GameTiming { class GameTimeManager; }
struct lua_State;

namespace LuaGameObjects
{
	constexpr const char* GLOBALGAMETIMEMANAGERNAME = "gameTimeManager";
	constexpr const char* STARTIDNAME = "startTimeID";
	constexpr const char* TARGETFRAMERATENAME = "targetFrameRate";
	constexpr const char* FRAMERATENAME = "frameRate";
	constexpr const char* GAMETIMENAME = "gameTime";

	class LuaGameTimeManager
	{
	public:
		static void Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager);
	
	private:
		static int getIndex(std::shared_ptr<Lua::LuaContext> luaContext);

		static int setIndex(std::shared_ptr<Lua::LuaContext> luaContext);
	};
}