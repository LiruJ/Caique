#include "Camera.h"

// GameObject includes.
#include "GameObject.h"
#include "Transform.h"

void Behaviours::Camera::Initialise(std::shared_ptr<Graphics::GraphicsContext> graphicsContext)
{
	onResize(graphicsContext->GetOutputWidth(), graphicsContext->GetOutputHeight());
	graphicsContext->ListenForResize(std::bind(&Behaviours::Camera::onResize, this, std::placeholders::_1, std::placeholders::_2));
}

glm::mat4 Behaviours::Camera::GetView() { return GetGameObject()->GetTransform()->GetInvertedGlobalMatrix(); }

glm::mat4 Behaviours::Camera::GetInvertedView() { return GetGameObject()->GetTransform()->GetGlobalMatrix(); }

void Behaviours::Camera::onResize(int newWidth, int newHeight)
{
	SetProjection(glm::perspective(glm::radians(45.0f), newWidth / (float)newHeight, 0.1f, 1000.0f));
	width = newWidth;
	height = newHeight;
}