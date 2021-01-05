#include "LuaInputManager.h"

// Lua includes.
#include "LuaContext.h"

// Input includes.
#include "InputManager.h"

void LuaGameObjects::LuaInputManager::Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<Input::InputManager> inputManager)
{
	// Start balancing.
	luaContext->BeginBalancing();

	// Create a userdata that holds a pointer to the inputManager. We get a pointer to the pointer back. Also store its stack position.
	std::weak_ptr<Input::InputManager>* inputManagerPointer = (std::weak_ptr<Input::InputManager>*)luaContext->PushUserData(sizeof(std::weak_ptr<Input::InputManager>));
	int inputManagerData = luaContext->GetTopIndex();

	// Placement operator, see Lua::LuaContext::Push(luaContextFunction) for more info.
	new(inputManagerPointer) std::weak_ptr<Input::InputManager>(inputManager);

	// Create a metatable for the userdata.
	int metatable = luaContext->PushNewMetatable(GLOBALINPUTMANAGERNAME);

	// Make the indexer just return the function.	
	luaContext->Push(indexFunction);
	luaContext->SetField(Lua::INDEXERNAME, metatable);

	// Set the metatable of the userdata to the created metatable.
	luaContext->SetMetatable(inputManagerData);

	// Add the userdata to the global namespace so that any script can access and use it.
	luaContext->SetGlobal(GLOBALINPUTMANAGERNAME);

	// Stop balancing.
	luaContext->StopBalancing();
}

int LuaGameObjects::LuaInputManager::indexFunction(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the function arguments, luckily they're the same as all the other operations so we can reuse this function.
	std::shared_ptr<Input::InputManager> inputManager = nullptr;
	const char* propertyName = nullptr;
	getFunctionArguments(luaContext, inputManager, propertyName);

	// Handle the desired function.
	if (strcmp(propertyName, GETKEYDOWNNAME) == 0)
		luaContext->Push(getKeyDown);
	else if (strcmp(propertyName, GETKEYUPNAME) == 0)
		luaContext->Push(getKeyUp);
	else if (strcmp(propertyName, ISKEYDOWNNAME) == 0)
		luaContext->Push(isKeyDown);
	else if (strcmp(propertyName, ISKEYUPNAME) == 0)
		luaContext->Push(isKeyUp);
	else if (strcmp(propertyName, WASKEYDOWNNAME) == 0)
		luaContext->Push(wasKeyDown);
	else if (strcmp(propertyName, WASKEYUPNAME) == 0)
		luaContext->Push(wasKeyUp);
	// Push nil onto the stack if the function does not exist.
	else 
		luaContext->PushNil();

	return 1;
}

int LuaGameObjects::LuaInputManager::getKeyDown(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the function arguments.
	std::shared_ptr<Input::InputManager> inputManager = nullptr;
	const char* keyName = nullptr;
	getFunctionArguments(luaContext, inputManager, keyName);

	// Push the result and return 1.
	luaContext->Push(inputManager->GetKeyDown(keyName));
	return 1;
}

int LuaGameObjects::LuaInputManager::getKeyUp(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the function arguments.
	std::shared_ptr<Input::InputManager> inputManager = nullptr;
	const char* keyName = nullptr;
	getFunctionArguments(luaContext, inputManager, keyName);

	// Push the result and return 1.
	luaContext->Push(inputManager->GetKeyUp(keyName));
	return 1;
}

int LuaGameObjects::LuaInputManager::isKeyDown(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the function arguments.
	std::shared_ptr<Input::InputManager> inputManager = nullptr;
	const char* keyName = nullptr;
	getFunctionArguments(luaContext, inputManager, keyName);

	// Push the result and return 1.
	luaContext->Push(inputManager->IsKeyDown(keyName));
	return 1;
}

int LuaGameObjects::LuaInputManager::isKeyUp(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the function arguments.
	std::shared_ptr<Input::InputManager> inputManager = nullptr;
	const char* keyName = nullptr;
	getFunctionArguments(luaContext, inputManager, keyName);

	// Push the result and return 1.
	luaContext->Push(inputManager->IsKeyUp(keyName));
	return 1;
}

int LuaGameObjects::LuaInputManager::wasKeyDown(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the function arguments.
	std::shared_ptr<Input::InputManager> inputManager = nullptr;
	const char* keyName = nullptr;
	getFunctionArguments(luaContext, inputManager, keyName);

	// Push the result and return 1.
	luaContext->Push(inputManager->WasKeyDown(keyName));
	return 1;
}

int LuaGameObjects::LuaInputManager::wasKeyUp(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the function arguments.
	std::shared_ptr<Input::InputManager> inputManager = nullptr;
	const char* keyName = nullptr;
	getFunctionArguments(luaContext, inputManager, keyName);

	// Push the result and return 1.
	luaContext->Push(inputManager->WasKeyUp(keyName));
	return 1;
}

void LuaGameObjects::LuaInputManager::getFunctionArguments(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<Input::InputManager>& inputManager, const char*& keyName)
{
	// There should be two arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount == 1 && luaContext->IsString(1)) luaContext->Error("expecting 2 arguments, the first of which being the inputManager. Did you mean to use a colon ':' instead of a dot '.'?");
	if (argumentCount != 2) luaContext->Error("expecting 2 arguments, instead was given %d arguments", argumentCount);

	// Get the inputManager, this should be the first argument.
	inputManager = ((std::weak_ptr<Input::InputManager>*)luaContext->CheckUserData(1, GLOBALINPUTMANAGERNAME))->lock();

	// The name of the key itself should be the second argument.
	keyName = luaContext->CheckCString(2);
}
