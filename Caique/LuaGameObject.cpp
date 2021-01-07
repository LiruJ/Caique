#include "LuaGameObject.h"

// Graphics includes.
#include <GraphicsContext.h>

// GameObject includes.
#include "GameObject.h"
#include "Transform.h"

// Behaviour includes.
#include "Camera.h"
#include "MeshRenderer.h"
#include "ScriptInstance.h"

// Lua includes.
#include "LuaContext.h"
#include "LuaTransform.h"
#include "LuaScript.h"
#include "LuaGraphicsContext.h"
#include "LuaVector3.h"
#include "LuaQuaternion.h"

void LuaGameObjects::LuaGameObject::Register(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Start balancing.
	luaContext->BeginBalancing();

	// Create a metatable to bind to.
	int gameObjectMetatable = luaContext->PushNewMetatable(GAMEOBJECTTYPENAME);

	// Bind get index.
	luaContext->Push(getIndex);
	luaContext->SetField(Lua::INDEXERNAME, gameObjectMetatable);

	// Remove the metatable.
	luaContext->Remove(gameObjectMetatable);

	// Stop balancing.
	luaContext->StopBalancing();
}

void LuaGameObjects::LuaGameObject::CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameObjects::GameObject> gameObject)
{
	// Create the userdata and set its value.
	std::weak_ptr<GameObjects::GameObject>* gameObjectPointer = (std::weak_ptr<GameObjects::GameObject>*)luaContext->PushUserData(sizeof(std::weak_ptr<GameObjects::GameObject>));
	int gameObjectData = luaContext->GetTopIndex();

	// Placement operator, see Lua::LuaContext::Push(luaContextFunction) for more info.
	new(gameObjectPointer) std::weak_ptr<GameObjects::GameObject>(gameObject);

	// Set the metatable for the data.
	luaContext->GetMetaTable(GAMEOBJECTTYPENAME);
	luaContext->SetMetatable(gameObjectData);
}

int LuaGameObjects::LuaGameObject::getIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// The first argument should be a gameObject.
	std::shared_ptr<GameObjects::GameObject> gameObject = ((std::weak_ptr<GameObjects::GameObject>*)luaContext->CheckUserData(1, GAMEOBJECTTYPENAME))->lock();

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// If the property is the transform, get it.
	if (propertyName == LuaGameObjects::TRANSFORMTYPENAME)
		LuaGameObjects::LuaTransform::CreateOnStack(luaContext, gameObject->GetTransform());
	// If the property is a function, forward it on.
	else if (propertyName == ADDCOMPONENTNAME)
		luaContext->Push(addComponent);
	else if (propertyName == ADDGAMEOBJECTNAME)
		luaContext->Push(addNewGameObject);
	// Finally; push a nil if nothing else was found.
	else luaContext->PushNil();

	return 1;
}

int LuaGameObjects::LuaGameObject::addComponent(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// The first argument should be a gameObject.
	std::shared_ptr<GameObjects::GameObject> gameObject = ((std::weak_ptr<GameObjects::GameObject>*)luaContext->CheckUserData(1, GAMEOBJECTTYPENAME))->lock();

	// The name of the component itself should be the second argument.
	std::string componentName = luaContext->CheckString(2);

	// Handle the component type.
	if (componentName == "MeshRenderer")
	{
		gameObject->AddComponent<Behaviours::MeshRenderer>(luaContext->CheckString(3));
		luaContext->PushNil();
	}
	else if (componentName == "Camera")
	{
		std::shared_ptr<Graphics::GraphicsContext> graphicsContext = ((std::weak_ptr<Graphics::GraphicsContext>*)luaContext->CheckUserData(3, LuaGameObjects::GLOBALGRAPHICSCONTEXTNAME))->lock();

		gameObject->AddComponent<Behaviours::Camera>(graphicsContext);
		luaContext->PushNil();
	}
	else if (componentName == "ScriptInstance")
	{
		// Create the script instance.
		std::shared_ptr<Behaviours::ScriptInstance> scriptInstance = gameObject->AddComponent<Behaviours::ScriptInstance>(luaContext->CheckString(3));

		// Get the environment table onto the stack.
		scriptInstance->GetScript()->GetEnvironment();
	}
	else luaContext->PushNil();
	
	// TODO: Return the component itself rather than just handling scripts.
	return 1;
}

int LuaGameObjects::LuaGameObject::addNewGameObject(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// The first argument should be a gameObject.
	std::shared_ptr<GameObjects::GameObject> gameObject = ((std::weak_ptr<GameObjects::GameObject>*)luaContext->CheckUserData(1, GAMEOBJECTTYPENAME))->lock();

	// Add a new gameObject to the gameObject.
	std::shared_ptr<GameObjects::GameObject> newGameObject = gameObject->AddNewGameObject();

	// If a second argument was given, treat it as a position.
	if (luaContext->GetTopIndex() == 2)
		newGameObject->GetTransform()->SetLocalPosition(*(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME));

	// If three arguments were given, treat them as position and rotation.
	if (luaContext->GetTopIndex() == 3)
	{
		newGameObject->GetTransform()->SetLocalPosition(*(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME));
		newGameObject->GetTransform()->SetLocalRotation(*(glm::quat*)luaContext->CheckUserData(3, QUATERNIONTYPENAME));
	}

	// Push the gameObject onto the stack.
	CreateOnStack(luaContext, newGameObject);

	return 1;
}
