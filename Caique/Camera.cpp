#include "Camera.h"

// GameObject includes.
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

Behaviours::Camera::Camera(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::ContentManager> contentManager, std::shared_ptr<Graphics::GraphicsContext> graphicsContext)
	: Behaviour(gameObject, contentManager), width(0), height(0), projection(glm::identity<glm::mat4>()), invertedProjection(glm::identity<glm::mat4>())
{

	onResize(graphicsContext->GetOutputWidth(), graphicsContext->GetOutputHeight());
	graphicsContext->ListenForResize(std::bind(&Behaviours::Camera::onResize, this, std::placeholders::_1, std::placeholders::_2));
}

void Behaviours::Camera::PreInitialise()
{
	gameObject.lock()->GetScene()->RegisterCamera(gameObject.lock()->GetComponent<Behaviours::Camera>());
}

glm::mat4 Behaviours::Camera::GetView() { return GetGameObject()->GetTransform()->GetInvertedGlobalMatrix(); }

glm::mat4 Behaviours::Camera::GetInvertedView() { return GetGameObject()->GetTransform()->GetGlobalMatrix(); }

void Behaviours::Camera::onResize(int newWidth, int newHeight)
{
	SetProjection(glm::perspective(glm::radians(45.0f), newWidth / (float)newHeight, 0.1f, 1000.0f));
	width = newWidth;
	height = newHeight;
}