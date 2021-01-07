#include "ScriptInstance.h"

#include <iostream>

// Timing includes.
#include "GameTime.h"

// Content includes.
#include "ContentManager.h"

// GameObject includes.
#include "GameObject.h"
#include "Transform.h"

// Lua includes.
#include "LuaScript.h"
#include "LuaGameTime.h"
#include "LuaTransform.h"
#include "LuaGameObject.h"

Behaviours::ScriptInstance::ScriptInstance(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::ContentManager> contentManager, const std::string& scriptName)
	: Behaviour(gameObject, contentManager)
{
	// Create a new instance of the script, loaded from the content manager.
	script = contentManager.lock()->Load<Lua::LuaScript>(scriptName);

	// Set the transform.
	LuaGameObjects::LuaTransform::CreateOnStack(script->GetLuaContext(), gameObject.lock()->GetTransform());
	script->SetEnvironmentField(LuaGameObjects::TRANSFORMTYPENAME, script->GetLuaContext()->GetTopIndex());

	// Set the gameObject.
	LuaGameObjects::LuaGameObject::CreateOnStack(script->GetLuaContext(), gameObject.lock());
	script->SetEnvironmentField(LuaGameObjects::GAMEOBJECTTYPENAME, script->GetLuaContext()->GetTopIndex());
}

void Behaviours::ScriptInstance::PreInitialise()
{
	// Perform setup on the script, fully setting it up ready to be used.
	script->Setup();

	// Run the preInitialise function.
	if (script->HasFunction("PreInitialise")) script->RunFunction("PreInitialise");
}

void Behaviours::ScriptInstance::Initialise()
{
	// Run the initialise function.
	if (script->HasFunction("Initialise")) script->RunFunction("Initialise");
}

void Behaviours::ScriptInstance::PostInitialise()
{
	// Run the postInitialise function.
	if (script->HasFunction("PostInitialise")) script->RunFunction("PostInitialise");
}

void Behaviours::ScriptInstance::Update(GameTiming::GameTime& gameTime)
{
	// If the script defines an update function, call it.
	if (script->HasFunction("Update"))
	{
		// Push the gametime object onto the stack.
		LuaGameObjects::LuaGameTime::CreateOnStack(script->GetLuaContext(), gameTime);

		// Try to run the update function using the stack, if it fails then the gametime object needs to be manually removed.
		if (!script->RunFunctionUsingStack("Update", 1))
			script->GetLuaContext()->Remove(-1);
	}
}

void Behaviours::ScriptInstance::PostUpdate()
{
	if (script->HasFunction("PostUpdate")) script->RunFunction("PostUpdate");
}