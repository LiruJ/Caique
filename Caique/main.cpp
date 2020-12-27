#include <SDL.h>
#include <iostream>
#include <string>

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

// GameObject includes.
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

int main(int argc, char* argv[])
{	
	// Create the graphics context.
	std::shared_ptr<Graphics::GraphicsContext> graphicsContext = std::make_shared<Graphics::GraphicsContext>();
	graphicsContext->Initialise(1920, 1000);

	// Create the content manager.
	std::shared_ptr<Content::ContentManager> contentManager = std::make_shared<Content::ContentManager>(argv[0], "Content", graphicsContext);

	// Create the scene.
	std::shared_ptr<GameObjects::Scene> scene = GameObjects::Scene::CreateScene(contentManager);
	std::shared_ptr<GameTiming::GameTimeManager> gameTimeManager = std::make_shared<GameTiming::GameTimeManager>();
	
	// Create the camera.
	std::shared_ptr<GameObjects::GameObject> cameraMount = scene->CreateGameObject();
	cameraMount->GetTransform()->SetLocalRotation(glm::quat(glm::vec3(-0.35877f, 0, 0)));

	std::shared_ptr<GameObjects::GameObject> cameraObject = scene->CreateGameObject();
	std::shared_ptr<Behaviours::Camera> camera = cameraObject->AddComponent<Behaviours::Camera>(graphicsContext->GetOutputWidth(), graphicsContext->GetOutputHeight());
	cameraMount->GetTransform()->AddChild(cameraObject->GetTransform());
	cameraObject->GetTransform()->SetLocalPosition(glm::vec3(0, 0, 21.0f));

	// Create the lamp model.
	std::shared_ptr<GameObjects::GameObject> lamp = scene->CreateGameObject();
	std::shared_ptr<Behaviours::MeshRenderer> lampMesh = lamp->AddComponent<Behaviours::MeshRenderer>(std::string("Models\\Lamp"));
	lamp->GetTransform()->SetLocalPosition(glm::vec3(0, -2, 0));

	scene->CreateGameObject()->AddComponent<Behaviours::MeshRenderer>(std::string("Models\\Desk"));

	bool quit = false;

	float rotationPerSeconds = 0.1f;
	std::string performanceStats;
	int maxFrameRate = 0;

	while (!quit)
	{
		gameTimeManager->Update();
		GameTiming::GameTime gameTime = gameTimeManager->GetCurrentGameTime();

		SDL_Event event = { 0 };

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		
		cameraMount->GetTransform()->SetLocalRotation(glm::quat(glm::vec3(0, rotationPerSeconds * gameTime.deltaSeconds, 0)) * cameraMount->GetTransform()->GetLocalRotation());

		if (gameTimeManager->GetSmoothedFrameRate() > maxFrameRate) maxFrameRate = gameTimeManager->GetSmoothedFrameRate();

		performanceStats = "Delta: " + std::to_string(floor(gameTime.deltaMilliseconds)) + "ms Max FPS: " + std::to_string(maxFrameRate) + " Current FPS: " + std::to_string(gameTimeManager->GetSmoothedFrameRate());
		graphicsContext->SetWindowTitle(performanceStats);

		// Clear the background.
		graphicsContext->Clear(0, 0, 0);

		scene->Draw(*camera);

		graphicsContext->Present();

		gameTimeManager->WaitFrameRemainder(*graphicsContext);
	}

	SDL_Quit();

	return 0;
}