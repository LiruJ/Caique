#include "LuaGameTimeManager.h"

// Lua includes.
#include "LuaContext.h"
#include "LuaGameTime.h"

// Timing includes.
#include "GameTimeManager.h"
#include "GameTime.h"

void LuaGameObjects::LuaGameTimeManager::Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager)
{
	// Start balancing.
	luaContext->BeginBalancing();

	// Create a userdata that holds a pointer to the gameTimeManager. We get a pointer to the pointer back. Also store its stack position.
	GameTiming::GameTimeManager** pointerUserData = (GameTiming::GameTimeManager**)luaContext->PushUserData(sizeof(gameTimeManager.get()));
	int userData = luaContext->GetTopIndex();

	// Set the userdata pointer to the gameTimeManager.
	*pointerUserData = gameTimeManager.get();

	// Create a metatable for the userdata.
	int metaTable = luaContext->PushNewMetaTable(GLOBALGAMETIMEMANAGERNAME);

	// Bind the read indexer to call the getter functions.
	luaContext->Push(GetIndex);
	luaContext->SetField(Lua::INDEXERNAME, metaTable);

	// Bind the assign indexer to call the setter functions.
	luaContext->Push(SetIndex);
	luaContext->SetField(Lua::NEWINDEXERNAME, metaTable);

	// Set the metatable of the userdata to the created metatable.
	luaContext->SetMetaTable(userData);

	// Add the userdata to the global namespace so that any script can access and use it.
	luaContext->SetGlobal(GLOBALGAMETIMEMANAGERNAME);

	// Stop balancing.
	luaContext->StopBalancing();
}

int LuaGameObjects::LuaGameTimeManager::SetIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the gameTimeManager, this should be the first argument.
	GameTiming::GameTimeManager* gameTimeManager = *static_cast<GameTiming::GameTimeManager**>(luaContext->ToUserData(1));

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);
	
	// Handle the property type.
	if (propertyName == TARGETFRAMERATENAME)
		gameTimeManager->SetTargetFrameRate(luaContext->CheckInt(-1));
		
	// In any case, this function should not put anything onto the stack.
	return 0;
}

int LuaGameObjects::LuaGameTimeManager::GetIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the gameTimeManager, this should be the first argument.
	GameTiming::GameTimeManager* gameTimeManager = *static_cast<GameTiming::GameTimeManager**>(luaContext->ToUserData(1));

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->ToString(2);

	// Handle the property type.
	if (propertyName == TARGETFRAMERATENAME)
	{
		luaContext->Push(gameTimeManager->GetTargetFrameRate());
		return 1;
	}
	else if (propertyName == FRAMERATENAME)
	{
		luaContext->Push(gameTimeManager->GetCurrentFrameRate());
		return 1;
	}
	else if (propertyName == GAMETIMENAME)
	{
		GameTiming::GameTime gameTime = gameTimeManager->GetCurrentGameTime();
		LuaGameObjects::LuaGameTime::CreateOnStack(luaContext, gameTime);
		return 1;
	}

	return 0;
}
