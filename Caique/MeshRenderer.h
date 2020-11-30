#pragma once

// Type includes.
#include <string>

// Derived includes.
#include "Behaviour.h"

// Graphics includes.
#include "Model.h"

namespace Behaviours
{
	class MeshRenderer : public Behaviour
	{
	public:
		void Initialise() override;

		void Draw(Behaviours::Camera& camera) override;

		std::shared_ptr<Graphics::Model> GetModel() { return model.lock(); }

		void SetModel(const std::string& name) { std::shared_ptr<Graphics::Model> tempModel; contentManager.lock()->LoadModel(name, tempModel); model = tempModel; }

		void SetModel(std::shared_ptr<Graphics::Model> newModel) { model = newModel; }
	private:
		std::weak_ptr<Graphics::Model> model;
	};
}