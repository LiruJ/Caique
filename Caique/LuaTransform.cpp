#include "LuaTransform.h"

// Transform includes.
#include "Transform.h"

// Lua includes.
#include "LuaContext.h"
#include "LuaVector3.h"

void LuaGameObjects::LuaTransform::Register(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Start balancing.
	luaContext->BeginBalancing();

	// Create a metatable to bind to.
	int transformMetatable = luaContext->PushNewMetaTable(TRANSFORMTYPENAME);

	// Bind get/set index.
	luaContext->Push(getIndex);
	luaContext->SetField(Lua::INDEXERNAME, transformMetatable);
	luaContext->Push(setIndex);
	luaContext->SetField(Lua::NEWINDEXERNAME, transformMetatable);

	// Remove the metatable.
	luaContext->Remove(transformMetatable);

	// Stop balancing.
	luaContext->StopBalancing();
}

void LuaGameObjects::LuaTransform::CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<GameObjects::Transform> transform)
{
	// Create the userdata and set its value.
	std::weak_ptr<GameObjects::Transform>* transformPointer = (std::weak_ptr<GameObjects::Transform>*)luaContext->PushUserData(sizeof(std::weak_ptr<GameObjects::Transform>));
	int transformData = luaContext->GetTopIndex();

	// Placement operator, see Lua::LuaContext::Push(luaContextFunction) for more info.
	new(transformPointer) std::weak_ptr<GameObjects::Transform>(transform);

	// Set the metatable for the data.
	luaContext->GetMetaTable(TRANSFORMTYPENAME);
	luaContext->SetMetaTable(transformData);
}

int LuaGameObjects::LuaTransform::getIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// The first argument should be a transform.
	std::shared_ptr<GameObjects::Transform> transform = ((std::weak_ptr<GameObjects::Transform>*)luaContext->CheckUserData(1, TRANSFORMTYPENAME))->lock();

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// If the property is the local position, get it.
	if (propertyName == LOCALPOSITIONNAME)
		LuaGameObjects::LuaVector3::CreateOnStack(luaContext, transform->GetLocalPosition());
	// Finally; push a nil if nothing else was found.
	else luaContext->PushNil();

	return 1;
}

int LuaGameObjects::LuaTransform::setIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be three arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 3)  luaContext->Error("set index was expecting 3 arguments, instead was given %d arguments", argumentCount);

	// The first argument should be a transform.
	std::shared_ptr<GameObjects::Transform> transform = ((std::weak_ptr<GameObjects::Transform>*)luaContext->CheckUserData(1, TRANSFORMTYPENAME))->lock();

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// If the property is the local position, set it.
	if (propertyName == LOCALPOSITIONNAME)
	{
		// Get the value as a vector and set the property.
		glm::vec3 value = *(glm::vec3*)luaContext->CheckUserData(3, VECTOR3TYPENAME);
		transform->SetLocalPosition(value);
	}

	return 0;
}