#pragma once

// Type includes.
#include <string>

// Behaviour includes.
#include "Behaviour.h"

// Forward declarations.
namespace Graphics { class Model; }

namespace Behaviours
{
	class MeshRenderer : public Behaviour
	{
	public:
		MeshRenderer(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::JsonContentManager> contentManager, const std::string& modelName);

		void PreInitialise() override;

		void Draw(Behaviours::Camera& camera);

		std::shared_ptr<Graphics::Model> GetModel() { return model.lock(); }

		void SetModel(const std::string& name);

		void SetModel(std::shared_ptr<Graphics::Model> newModel) { model = newModel; }
	private:
		std::weak_ptr<Graphics::Model> model;
	};
}