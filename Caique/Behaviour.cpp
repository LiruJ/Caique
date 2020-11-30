#include "Behaviour.h"

// Forward declare includes.
#include "Camera.h"

void Behaviours::Behaviour::Setup(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::ContentManager> contentManager)
{
	this->gameObject = gameObject;
	this->contentManager = contentManager;
}
