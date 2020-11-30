#include "MeshRenderer.h"

// Transform is forward declared in GameObject.h which is where this file knows of Transform otherwise, so get the full thing here.
#include "Transform.h"

#include "Camera.h"

void Behaviours::MeshRenderer::Initialise()
{
	//std::shared_ptr<Graphics::Model> temp;
	//contentManager.lock()->LoadModel("Models/Lamp", temp);
	//model = temp;
}

void Behaviours::MeshRenderer::Draw(Behaviours::Camera& camera)
{
	std::shared_ptr<Graphics::Model> drawModel = GetModel();
	if (drawModel != nullptr)
		drawModel->Draw(GetGameObject()->GetTransform()->GetGlobalMatrix(), camera.GetProjection(), camera.GetView());
}