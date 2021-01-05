#include <SDL.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

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
	
	// Create the scene.
	std::shared_ptr<GameObjects::Scene> scene = GameObjects::Scene::CreateScene(contentManager);
	std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager = std::make_shared<GameTiming::GameTimeManager>();
	LuaGameObjects::LuaGameTimeManager::Register(luaContext, gameTimeManager);

	// Create the camera.
	std::shared_ptr<GameObjects::GameObject> cameraMount = scene->CreateGameObject();
	cameraMount->GetTransform()->SetLocalRotation(glm::quat(glm::vec3(-0.35877f, 0, 0)));

	std::shared_ptr<GameObjects::GameObject> cameraObject = scene->CreateGameObject();
	std::shared_ptr<Behaviours::Camera> camera = cameraObject->AddComponent<Behaviours::Camera>(graphicsContext->GetOutputWidth(), graphicsContext->GetOutputHeight());
	cameraMount->GetTransform()->AddChild(cameraObject->GetTransform());
	cameraObject->GetTransform()->SetLocalPosition(glm::vec3(0, 0, 21.0f));
	cameraMount->AddComponent<Behaviours::ScriptInstance>("Scripts\\CameraRotator");

	// Create the lamp model.
	std::shared_ptr<GameObjects::GameObject> lamp = scene->CreateGameObject();
	std::shared_ptr<Behaviours::MeshRenderer> lampMesh = lamp->AddComponent<Behaviours::MeshRenderer>(std::string("Models\\Lamp"));
	lamp->GetTransform()->SetLocalPosition(glm::vec3(0, -2, 0));
	lamp->AddComponent<Behaviours::ScriptInstance>("Scripts\\FPSTracker");

	scene->CreateGameObject()->AddComponent<Behaviours::MeshRenderer>(std::string("Models\\Desk"));

	bool quit = false;
	while (!quit)
	{
		// Update the gametime.
		gameTimeManager->Update();
		GameTiming::GameTime gameTime = gameTimeManager->GetCurrentGameTime();

		// TODO: InputManager.
		SDL_Event event = { 0 };

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

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