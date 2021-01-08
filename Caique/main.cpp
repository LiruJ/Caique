#include <glm\glm.hpp>

// Event includes.
#include "SDLEventManager.h"
#include "SDLInputManager.h"

// Timing includes.
#include "SDLGameTimeManager.h"
#include "GameTime.h"

// Content includes.
#include "JsonContentManager.h"

// Graphics includes.
#include <GraphicsContext.h>
#include "Model.h"

// Behaviour includes.
#include "Camera.h"
#include "MeshRenderer.h"
#include "ScriptInstance.h"

// GameObject includes.
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

// Lua includes.
#include "LuaContext.h"
#include "LuaScript.h"
#include "LuaGameTimeManager.h"
#include "LuaGraphicsContext.h"
#include "LuaVector3.h"
#include "LuaQuaternion.h"
#include "LuaTransform.h"
#include "LuaInputManager.h"
#include "LuaGameObject.h"
#include "LuaScene.h"

#include <iostream>

int main(int argc, char* argv[])
{	
	// Create the Lua context.
	std::shared_ptr<Lua::LuaContext> luaContext = Lua::LuaContext::Create();
	Lua::LuaScript::Register(luaContext);
	LuaGameObjects::LuaVector3::Register(luaContext);
	LuaGameObjects::LuaQuaternion::Register(luaContext);
	LuaGameObjects::LuaTransform::Register(luaContext);
	LuaGameObjects::LuaGameObject::Register(luaContext);

	// Create the graphics context.
	std::shared_ptr<Graphics::GraphicsContext> graphicsContext = std::make_shared<Graphics::GraphicsContext>();
	graphicsContext->Initialise(1920, 1000);
	LuaGameObjects::LuaGraphicsContext::Register(luaContext, graphicsContext);

	// Create the content manager.
	std::shared_ptr<Content::JsonContentManager> contentManager = std::make_shared<Content::JsonContentManager>(argv[0], "Content", graphicsContext, luaContext);
	
	// Create the input and events managers.
	std::shared_ptr<Input::SDLInputManager> sdlInputManager = std::make_shared<Input::SDLInputManager>(graphicsContext);
	std::shared_ptr<Input::InputManager> inputManager = sdlInputManager;

	std::shared_ptr<Events::EventManager> eventManager = std::make_shared<Events::SDLEventManager>(sdlInputManager, graphicsContext);
	LuaGameObjects::LuaInputManager::Register(luaContext, inputManager);

	// Create the scene.
	std::shared_ptr<GameObjects::Scene> scene = GameObjects::Scene::CreateScene(contentManager);
	std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager = std::make_shared<GameTiming::SDLGameTimeManager>();
	LuaGameObjects::LuaGameTimeManager::Register(luaContext, gameTimeManager);
	LuaGameObjects::LuaScene::Register(luaContext, scene);

	// Load the scene from file.
	std::shared_ptr<Lua::LuaScript> loadScript = contentManager->Load<Lua::LuaScript>("Scripts\\SceneLoader");
	loadScript->Setup();

	// Initialise the scene.
	scene->Initialise();
	
	while (!eventManager->IsQuitting())
	{
		// Update the gametime.
		gameTimeManager->Update();
		GameTiming::GameTime gameTime = gameTimeManager->GetCurrentGameTime();

		// Update events, which also updates inputs.
		eventManager->Update();
		
		// Update the scene using the GameTime, this updates every GameObject and their Behaviours.
		scene->Update(gameTime);
		
		// Draw the scene.
		graphicsContext->Clear(0, 0, 0);
		scene->Draw();
		graphicsContext->Present();
		
		// Wait out the remaining frame time.
		gameTimeManager->WaitFrameRemainder(*graphicsContext);
	}

	SDL_Quit();

	return 0;
}