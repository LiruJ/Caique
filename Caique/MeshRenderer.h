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
		void Initialise() {}
		void Initialise(std::string&& modelName);

		void Draw(Behaviours::Camera& camera) override;

		std::shared_ptr<Graphics::Model> GetModel() { return model.lock(); }

		void SetModel(const std::string& name);

		void SetModel(std::shared_ptr<Graphics::Model> newModel) { model = newModel; }
	private:
		std::weak_ptr<Graphics::Model> model;
	};
}