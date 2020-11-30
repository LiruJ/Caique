#include "Scene.h"

std::shared_ptr<GameObjects::Scene> GameObjects::Scene::CreateScene(std::weak_ptr<Content::ContentManager> contentManager)
{
	return std::shared_ptr<GameObjects::Scene>(new Scene(contentManager));
}

std::shared_ptr<GameObjects::GameObject> GameObjects::Scene::CreateGameObject()
{
	std::shared_ptr<GameObjects::GameObject> newGameObject = std::make_shared<GameObjects::GameObject>(contentManager);

	newGameObject->PreInitialise();
	newGameObject->Initialise();
	newGameObject->PostInitialise();

	gameObjects.push_back(newGameObject);

	return newGameObject;
}

void GameObjects::Scene::Draw(Behaviours::Camera& camera)
{
	for (int i = 0; i < gameObjects.size(); i++)
		gameObjects.at(i)->Draw(camera);
}
