#include "LuaGameTime.h"

// Lua includes.
#include "LuaContext.h"

// Timing includes.
#include "GameTime.h"

void LuaGameObjects::LuaGameTime::CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, GameTiming::GameTime& gameTime)
{
	// Make a new table on the stack, then fill it with the variables from the gametime.
	luaContext->PushNewTable();
	luaContext->Push(gameTime.deltaMilliseconds);
	luaContext->SetField("deltaMilliseconds", -2);

	luaContext->Push(gameTime.deltaSeconds);
	luaContext->SetField("deltaSeconds", -2);

	luaContext->Push(gameTime.millisecondsSinceStart);
	luaContext->SetField("millisecondsSinceStart", -2);

	luaContext->Push(gameTime.secondsSinceStart);
	luaContext->SetField("secondsSinceStart", -2);

	// Just cache the FPS as it's not going to change and binding an entire function to get it would be wasteful.
	luaContext->Push(gameTime.GetFramesPerSecond());
	luaContext->SetField("framesPerSecond", -2);
}