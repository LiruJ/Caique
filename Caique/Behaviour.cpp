#include "Behaviour.h"

// GameObject includes.
#include "GameObject.h"

// Behaviour includes.
#include "Camera.h"

// Content includes.
#include "ContentManager.h"

void Behaviours::Behaviour::Setup(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::ContentManager> contentManager)
{
	this->gameObject = gameObject;
	this->contentManager = contentManager;
}
