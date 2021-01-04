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

void Behaviours::ScriptInstance::Initialise(const std::string& scriptAsset)
{
	// Create a new instance of the script, loaded from the content manager.
	script = contentManager.lock()->Load<Lua::LuaScript>(scriptAsset);

	// Set the transform.
	LuaGameObjects::LuaTransform::CreateOnStack(script->GetLuaContext(), gameObject.lock()->GetTransform());
	script->SetEnvironmentField(LuaGameObjects::TRANSFORMTYPENAME, script->GetLuaContext()->GetTopIndex());

	// Perform setup on the script, fully setting it up ready to be used.
	script->Setup();
	
	// Run the initialise function.
	if (script->HasFunction("Initialise")) script->RunFunction("Initialise");
}

void Behaviours::ScriptInstance::PostInitialise()
{
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

void Behaviours::ScriptInstance::Draw(Behaviours::Camera& camera)
{
	if (script->HasFunction("Draw")) script->RunFunction("Draw");
}