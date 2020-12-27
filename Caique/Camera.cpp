#include "Camera.h"

// GameObject includes.
#include "GameObject.h"
#include "Transform.h"

// Graphics includes.
#include "GraphicsContext.h"

void Behaviours::Camera::Initialise(int windowWidth, int windowHeight)
{
	SetProjection(glm::perspective(glm::radians(45.0f), windowWidth / (float)windowHeight, 0.1f, 1000.0f));
	width = windowWidth;
	height = windowHeight;
}

void Behaviours::Camera::Initialise(std::shared_ptr<Graphics::GraphicsContext> graphicsContext, glm::mat4&& projection)
{
	width = graphicsContext->GetOutputWidth();
	height = graphicsContext->GetOutputHeight();
	SetProjection(projection);
}

glm::mat4 Behaviours::Camera::GetView() { return GetGameObject()->GetTransform()->GetInvertedGlobalMatrix(); }

glm::mat4 Behaviours::Camera::GetInvertedView() { return GetGameObject()->GetTransform()->GetGlobalMatrix(); }
