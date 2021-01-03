#include "Scene.h"

// Timing includes.
#include "GameTime.h"

// GameObject includes.
#include "GameObject.h"

// Content includes.
#include "ContentManager.h"

// Graphical includes.
#include "Camera.h"

std::shared_ptr<GameObjects::Scene> GameObjects::Scene::CreateScene(std::weak_ptr<Content::ContentManager> contentManager)
{
	return std::shared_ptr<GameObjects::Scene>(new Scene(contentManager));
}

std::shared_ptr<GameObjects::GameObject> GameObjects::Scene::CreateGameObject()
{
	std::shared_ptr<GameObjects::GameObject> newGameObject = std::make_shared<GameObjects::GameObject>(weak_from_this(), contentManager);

	newGameObject->PreInitialise();
	newGameObject->Initialise();
	newGameObject->PostInitialise();

	gameObjects.push_back(newGameObject);

	return newGameObject;
}

void GameObjects::Scene::Update(GameTiming::GameTime& gameTime)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects.at(i)->Update(gameTime);
}

void GameObjects::Scene::Draw(Behaviours::Camera& camera)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects.at(i)->Draw(camera);
}
