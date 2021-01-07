#include "Scene.h"

// Timing includes.
#include "GameTime.h"

// GameObject includes.
#include "GameObject.h"

// Content includes.
#include "ContentManager.h"

// Behaviour includes.
#include "Camera.h"
#include "MeshRenderer.h"

std::shared_ptr<GameObjects::Scene> GameObjects::Scene::CreateScene(std::weak_ptr<Content::ContentManager> contentManager)
{
	return std::shared_ptr<GameObjects::Scene>(new Scene(contentManager));
}

std::shared_ptr<GameObjects::GameObject> GameObjects::Scene::CreateNewGameObject()
{
	std::shared_ptr<GameObjects::GameObject> newGameObject = std::make_shared<GameObjects::GameObject>(weak_from_this(), contentManager);
	newGameObject->InitialiseTransform();

	// If the scene has already initialised, then immediately initialise this new gameobject.
	if (hasInitialised)
	{
		newGameObject->PreInitialise();
		newGameObject->Initialise();
		newGameObject->PostInitialise();
	}

	allGameObjects.push_back(newGameObject);

	return newGameObject;
}

std::shared_ptr<GameObjects::GameObject> GameObjects::Scene::AddNewGameObject()
{
	std::shared_ptr<GameObjects::GameObject> newGameObject = CreateNewGameObject();

	gameObjects.push_back(newGameObject);

	return newGameObject;
}

void GameObjects::Scene::RegisterCamera(std::shared_ptr<Behaviours::Camera> camera)
{
	cameras.push_back(camera);
}

void GameObjects::Scene::RegisterDrawable(std::shared_ptr<Behaviours::MeshRenderer> drawableObject)
{
	drawableObjects.push_back(drawableObject);
}

void GameObjects::Scene::Update(GameTiming::GameTime& gameTime)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects.at(i)->Update(gameTime);
}

void GameObjects::Scene::Draw()
{
	// Go over each registered camera.
	for (size_t i = 0; i < cameras.size(); i++)
	{
		Behaviours::Camera& camera = *cameras.at(i).lock();
		// Go over each registered drawable.
		for (size_t i = 0; i < drawableObjects.size(); i++)
			// Draw the drawable
			drawableObjects.at(i).lock()->Draw(camera);
	}
}

void GameObjects::Scene::Initialise()
{
	// If initialisation has already happened, do nothing.
	if (hasInitialised) return;

	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects.at(i)->PreInitialise();
	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects.at(i)->Initialise();
	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects.at(i)->PostInitialise();

	// The scene has now initialised.
	hasInitialised = true;
}