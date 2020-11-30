#include <SDL.h>
#include <GraphicsContext.h>
#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ContentManager.h"
#include "Model.h"

#include "Scene.h"

#include "Camera.h"
#include "MeshRenderer.h"
#include "GameObject.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::exception("SDL could not be initialised.");
	
	SDL_Window* window = SDL_CreateWindow("2D Cuboid",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	if (!SDL_GL_CreateContext(window))
		throw std::exception("OpenGL SDL linked incorrectly.");

	// Create the graphics context.
	Graphics::GraphicsContext graphicsContext = Graphics::GraphicsContext();
	graphicsContext.Initialise();

	// Create the content manager.
	std::shared_ptr<Content::ContentManager> contentManager = std::make_shared<Content::ContentManager>(argv[0], "Content", std::make_shared<Graphics::GraphicsContext>(graphicsContext));

	// Create the scene.
	std::shared_ptr<GameObjects::Scene> scene(GameObjects::Scene::CreateScene(contentManager));

	// Create the camera.
	std::shared_ptr<GameObjects::GameObject> cameraObject = scene->CreateGameObject();
	std::shared_ptr<Behaviours::Camera> camera = cameraObject->AddComponent<Behaviours::Camera>();
	camera->SetProjection(glm::perspective(45.0f, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f));
	cameraObject->GetTransform()->SetLocalPosition(glm::vec3(0, 0, 10));

	// Create the lamp model.
	std::shared_ptr<GameObjects::GameObject> lamp = scene->CreateGameObject();
	std::shared_ptr<Behaviours::MeshRenderer> lampMesh = lamp->AddComponent<Behaviours::MeshRenderer>();
	lampMesh->SetModel("Models\\Lamp");
	lamp->GetTransform()->SetLocalPosition(glm::vec3(0, -2, 0));

	lamp->GetTransform()->AddChild(cameraObject->GetTransform());

	scene->CreateGameObject()->AddComponent<Behaviours::MeshRenderer>()->SetModel("Models\\Lamp");

	bool quit = false;

	glm::vec3 moveDirection(0.01, 0, 0);

	while (!quit)
	{
		SDL_Event event = { 0 };

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		if (lamp->GetTransform()->GetLocalPosition().x > 5) moveDirection = glm::vec3(-0.01, 0, 0);
		else if (lamp->GetTransform()->GetLocalPosition().x < -5) moveDirection = glm::vec3(0.01, 0, 0);

		lamp->GetTransform()->SetLocalPosition(lamp->GetTransform()->GetLocalPosition() + moveDirection);
		//rotation += 0.3f;

		//modelMatrix = glm::mat4(1.0f);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -3.5, -10));
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));

		// Clear the background.
		graphicsContext.Clear(0, 0, 0);

		//lamp->Draw(*camera);
		scene->Draw(*camera);

		//lampModel->Draw(modelMatrix, projection, view);

		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}