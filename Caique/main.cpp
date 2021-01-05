#include <glm\glm.hpp>

// Event includes.
#include "EventManager.h"
#include "InputManager.h"

// Timing includes.
#include "GameTimeManager.h"
#include "GameTime.h"

// Content includes.
#include "ContentManager.h"

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

#include <iostream>

int main(int argc, char* argv[])
{	
	// Create the Lua context.
	std::shared_ptr<Lua::LuaContext> luaContext = Lua::LuaContext::Create();
	Lua::LuaScript::Register(luaContext);
	LuaGameObjects::LuaVector3::Register(luaContext);
	LuaGameObjects::LuaQuaternion::Register(luaContext);
	LuaGameObjects::LuaTransform::Register(luaContext);

	// Create the graphics context.
	std::shared_ptr<Graphics::GraphicsContext> graphicsContext = std::make_shared<Graphics::GraphicsContext>();
	graphicsContext->Initialise(1920, 1000);
	LuaGameObjects::LuaGraphicsContext::Register(luaContext, graphicsContext);

	// Create the content manager.
	std::shared_ptr<Content::ContentManager> contentManager = std::make_shared<Content::ContentManager>(argv[0], "Content", graphicsContext, luaContext);
	
	// Create the input and events managers.
	std::shared_ptr<Input::InputManager> inputManager = std::make_shared<Input::InputManager>();
	std::shared_ptr<Events::EventManager> eventManager = std::make_shared<Events::EventManager>(inputManager);
	LuaGameObjects::LuaInputManager::Register(luaContext, inputManager);

	// Create the scene.
	std::shared_ptr<GameObjects::Scene> scene = GameObjects::Scene::CreateScene(contentManager);
	std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager = std::make_shared<GameTiming::GameTimeManager>();
	LuaGameObjects::LuaGameTimeManager::Register(luaContext, gameTimeManager);

	// Create the camera.
	std::shared_ptr<GameObjects::GameObject> cameraObject = scene->CreateGameObject();
	std::shared_ptr<Behaviours::Camera> camera = cameraObject->AddComponent<Behaviours::Camera>(graphicsContext->GetOutputWidth(), graphicsContext->GetOutputHeight());

	cameraObject->AddComponent<Behaviours::ScriptInstance>("Scripts\\CameraRotator");
	cameraObject->GetTransform()->SetLocalRotation(glm::quat(glm::vec3(-0.35877f, 0, 0)));
	cameraObject->GetTransform()->SetLocalPosition(cameraObject->GetTransform()->GetLocalRotation() * glm::vec3(0, 0, 21.0f));

	// Create the lamp model.
	std::shared_ptr<GameObjects::GameObject> lamp = scene->CreateGameObject();
	std::shared_ptr<Behaviours::MeshRenderer> lampMesh = lamp->AddComponent<Behaviours::MeshRenderer>(std::string("Models\\Lamp"));
	lamp->GetTransform()->SetLocalPosition(glm::vec3(0, -2, 0));
	lamp->AddComponent<Behaviours::ScriptInstance>("Scripts\\FPSTracker");

	scene->CreateGameObject()->AddComponent<Behaviours::MeshRenderer>(std::string("Models\\Desk"));

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
		scene->Draw(*camera);
		graphicsContext->Present();

		// Wait out the remaining frame time.
		gameTimeManager->WaitFrameRemainder(*graphicsContext);
	}

	SDL_Quit();

	return 0;
}