#include "GameObject.h"

// Timing includes.
#include "GameTime.h"

// Behaviour includes.
#include "Behaviour.h"
#include "Transform.h"
#include "Camera.h"

// Content includes.
#include "JsonContentManager.h"

GameObjects::GameObject::GameObject(std::weak_ptr<GameObjects::Scene> scene, std::weak_ptr<Content::JsonContentManager> contentManager)
	: scene(scene), contentManager(contentManager)
{
}

void GameObjects::GameObject::InitialiseTransform()
{
	transform = std::make_shared<GameObjects::Transform>(shared_from_this());
}

std::shared_ptr<GameObjects::GameObject> GameObjects::GameObject::AddNewGameObject()
{
	std::shared_ptr<GameObjects::GameObject> newGameObject = scene.lock()->CreateNewGameObject();

	transform->AddChild(newGameObject->GetTransform());

	return newGameObject;
}

void GameObjects::GameObject::PreInitialise()
{
	// Pre-initialise all children first, then the behaviours. This has a top-down effect where the furthest children's components are the first to pre-initialise.
	for (size_t i = 0; i < transform->GetChildCount(); i++)
		transform->GetChildByIndex(i)->GetGameObject()->PreInitialise();
	for (auto& pair : behavioursByTypeIndex)
		pair.second->PreInitialise();
}

void GameObjects::GameObject::Initialise()
{
	// Initialise all behaviours first, then children. This has a bottom-up effect where the components of the scene's gameobjects are the first to initialise.
	for (auto& pair : behavioursByTypeIndex)
		pair.second->Initialise();
	for (size_t i = 0; i < transform->GetChildCount(); i++)
		transform->GetChildByIndex(i)->GetGameObject()->Initialise();
}

void GameObjects::GameObject::PostInitialise()
{
	// Post-initialise all children first, then the behaviours. This has a top-down effect where the furthest children's components are the first to post-initialise.
	for (size_t i = 0; i < transform->GetChildCount(); i++)
		transform->GetChildByIndex(i)->GetGameObject()->PostInitialise();
	for (auto& pair : behavioursByTypeIndex)
		pair.second->PostInitialise();
}

void GameObjects::GameObject::Update(GameTiming::GameTime& gameTime)
{
	for (auto& pair : behavioursByTypeIndex)
		pair.second->Update(gameTime);
	for (size_t i = 0; i < transform->GetChildCount(); i++)
		transform->GetChildByIndex(i)->GetGameObject()->Update(gameTime);
}

void GameObjects::GameObject::PostUpdate()
{
	for (auto& pair : behavioursByTypeIndex)
		pair.second->PostUpdate();
	for (size_t i = 0; i < transform->GetChildCount(); i++)
		transform->GetChildByIndex(i)->GetGameObject()->PostUpdate();
}

std::shared_ptr<GameObjects::Transform> GameObjects::GameObject::GetTransform() { return transform; }