#include "GameObject.h"

// Behaviour includes.
#include "Behaviour.h"
#include "Transform.h"
#include "Camera.h"

// Content includes.
#include "ContentManager.h"

// Scene includes.
#include "Scene.h"

void GameObjects::GameObject::PreInitialise()
{
	// Initialise the transform.
	transform = std::make_shared<GameObjects::Transform>(weak_from_this());

	// Pre-initialise all behaviours.
	for (auto& pair : behavioursByTypeIndex)
		pair.second->PreInitialise();
}

void GameObjects::GameObject::Initialise()
{
	for (auto& pair : behavioursByTypeIndex)
		pair.second->Initialise();
}

void GameObjects::GameObject::PostInitialise()
{
	for (auto& pair : behavioursByTypeIndex)
		pair.second->PostInitialise();
}

void GameObjects::GameObject::Update()
{
	for (auto& pair : behavioursByTypeIndex)
		pair.second->Update();
}

void GameObjects::GameObject::PostUpdate()
{
	for (auto& pair : behavioursByTypeIndex)
		pair.second->PostUpdate();
}

void GameObjects::GameObject::Draw(Behaviours::Camera& camera)
{
	for (auto& pair : behavioursByTypeIndex)
		pair.second->Draw(camera);

	for (int i = 0; i < transform->GetChildCount(); i++)
		transform->GetChildByIndex(i)->GetGameObject()->Draw(camera);
}

std::shared_ptr<GameObjects::Transform> GameObjects::GameObject::GetTransform() { return transform; }