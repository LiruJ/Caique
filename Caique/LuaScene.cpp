#include "LuaScene.h"

// GLM includes.
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

// GameObject includes.
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

// Lua includes.
#include "LuaContext.h"
#include "LuaGameObject.h"
#include "LuaVector3.h"
#include "LuaQuaternion.h"

void LuaGameObjects::LuaScene::Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameObjects::Scene> scene)
{
	// Start balancing.
	luaContext->BeginBalancing();
	
	// Create a userdata that holds a pointer to the scene. We get a pointer to the pointer back. Also store its stack position.
	std::weak_ptr<GameObjects::Scene>* scenePointer = (std::weak_ptr<GameObjects::Scene>*)luaContext->PushUserData(sizeof(std::weak_ptr<GameObjects::Scene>));
	int scenePointerData = luaContext->GetTopIndex();

	// Placement operator, see Lua::LuaContext::Push(luaContextFunction) for more info.
	new(scenePointer) std::weak_ptr<GameObjects::Scene>(scene);

	// Create a metatable for the userdata.
	int metatable = luaContext->PushNewMetatable(SCENETYPENAME);

	// Bind the read indexer to call the getter functions.
	luaContext->Push(getIndex);
	luaContext->SetField(Lua::INDEXERNAME, metatable);

	// Set the metatable of the userdata to the created metatable.
	luaContext->SetMetatable(scenePointerData);

	// Add the userdata to the global namespace so that any script can access and use it.
	luaContext->SetGlobal(SCENETYPENAME);

	// Stop balancing.
	luaContext->StopBalancing();
}

int LuaGameObjects::LuaScene::getIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// The first argument should be a scene.
	std::shared_ptr<GameObjects::Scene> scene = ((std::weak_ptr<GameObjects::Scene>*)luaContext->CheckUserData(1, SCENETYPENAME))->lock();

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// Handle the property name.
	if (propertyName == ADDSCENEGAMEOBJECTNAME)
		luaContext->Push(addNewGameObject);
	else luaContext->PushNil();

	return 1;
}

int LuaGameObjects::LuaScene::addNewGameObject(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// The first argument should be a scene.
	std::shared_ptr<GameObjects::Scene> scene = ((std::weak_ptr<GameObjects::Scene>*)luaContext->CheckUserData(1, SCENETYPENAME))->lock();

	// Add a new gameObject to the scene.
	std::shared_ptr<GameObjects::GameObject> gameObject = scene->AddNewGameObject();

	// If a second argument was given, treat it as a position.
	if (luaContext->GetTopIndex() == 2)
		gameObject->GetTransform()->SetLocalPosition(*(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME));

	// If three arguments were given, treat them as position and rotation.
	if (luaContext->GetTopIndex() == 3)
	{
		gameObject->GetTransform()->SetLocalPosition(*(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME));
		gameObject->GetTransform()->SetLocalRotation(*(glm::quat*)luaContext->CheckUserData(3, QUATERNIONTYPENAME));
	}

	// Push the gameObject onto the stack.
	LuaGameObjects::LuaGameObject::CreateOnStack(luaContext, gameObject);

	return 1;
}
