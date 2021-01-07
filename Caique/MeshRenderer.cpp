#include "MeshRenderer.h"

// GameObject includes.
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

// Behaviour includes.
#include "Camera.h"

// Graphics includes.
#include "Model.h"

// Content includes.
#include "ContentManager.h"

Behaviours::MeshRenderer::MeshRenderer(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::ContentManager> contentManager, const std::string& modelName)
	: Behaviour(gameObject, contentManager)
{
	// If the given string is empty, don't do anything with it.
	if (modelName.size() == 0) return;

	// Load the model.
	SetModel(modelName);
}

void Behaviours::MeshRenderer::PreInitialise()
{
	gameObject.lock()->GetScene()->RegisterDrawable(gameObject.lock()->GetComponent<Behaviours::MeshRenderer>());
}

void Behaviours::MeshRenderer::Draw(Behaviours::Camera& camera)
{
	std::shared_ptr<Graphics::Model> drawModel = GetModel();
	if (drawModel != nullptr)
		drawModel->Draw(GetGameObject()->GetTransform()->GetGlobalMatrix(), camera.GetProjection(), camera.GetView());
}

void Behaviours::MeshRenderer::SetModel(const std::string& name) 
{
	model = contentManager.lock()->Load<Graphics::Model>(name);
}