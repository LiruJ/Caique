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
	std::weak_ptr<GameTiming::GameTimeManager>* gameTimeManagerPointer = (std::weak_ptr<GameTiming::GameTimeManager>*)luaContext->PushUserData(sizeof(std::weak_ptr<GameTiming::GameTimeManager>));
	int gameTimeManagerData = luaContext->GetTopIndex();

	// Placement operator, see Lua::LuaContext::Push(luaContextFunction) for more info.
	new(gameTimeManagerPointer) std::weak_ptr<GameTiming::GameTimeManager>(gameTimeManager);

	// Create a metatable for the userdata.
	int metatable = luaContext->PushNewMetatable(GLOBALGAMETIMEMANAGERNAME);

	// Bind the read indexer to call the getter functions.
	luaContext->Push(getIndex);
	luaContext->SetField(Lua::INDEXERNAME, metatable);

	// Bind the assign indexer to call the setter functions.
	luaContext->Push(setIndex);
	luaContext->SetField(Lua::NEWINDEXERNAME, metatable);

	// Set the metatable of the userdata to the created metatable.
	luaContext->SetMetatable(gameTimeManagerData);

	// Add the userdata to the global namespace so that any script can access and use it.
	luaContext->SetGlobal(GLOBALGAMETIMEMANAGERNAME);

	// Stop balancing.
	luaContext->StopBalancing();
}

int LuaGameObjects::LuaGameTimeManager::getIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the gameTimeManager, this should be the first argument.
	std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager = ((std::weak_ptr<GameTiming::GameTimeManager>*)luaContext->CheckUserData(1, GLOBALGAMETIMEMANAGERNAME))->lock();

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// Handle the property type.
	if (propertyName == TARGETFRAMERATENAME)
		luaContext->Push(gameTimeManager->GetTargetFrameRate());
	else if (propertyName == FRAMERATENAME)
		luaContext->Push(gameTimeManager->GetCurrentFrameRate());
	else if (propertyName == GAMETIMENAME)
	{
		GameTiming::GameTime gameTime = gameTimeManager->GetCurrentGameTime();
		LuaGameObjects::LuaGameTime::CreateOnStack(luaContext, gameTime);
	}
	else luaContext->PushNil();

	return 1;
}

int LuaGameObjects::LuaGameTimeManager::setIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the gameTimeManager, this should be the first argument.
	std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager = ((std::weak_ptr<GameTiming::GameTimeManager>*)luaContext->CheckUserData(1, GLOBALGAMETIMEMANAGERNAME))->lock();

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);
	
	// Handle the property type.
	if (propertyName == TARGETFRAMERATENAME)
		gameTimeManager->SetTargetFrameRate(luaContext->CheckInt(-1));
		
	// In any case, this function should not put anything onto the stack.
	return 0;
}