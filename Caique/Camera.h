#pragma once

// Behaviour includes.
#include "Behaviour.h"

// GLM includes.
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

// Type includes.
#include <memory>

// Foward declarations.
namespace Graphics { class GraphicsContext; }

namespace Behaviours
{
	class Camera : public Behaviour
	{
	public:
		Camera() : width(0), height(0), projection(glm::identity<glm::mat4>()), invertedProjection(glm::identity<glm::mat4>()) { }

		glm::mat4 GetProjection() { return projection; }
		glm::mat4 GetInvertedProjection() { return invertedProjection; }
		void SetProjection(glm::mat4 projection) { this->projection = projection; invertedProjection = glm::inverse(projection); }

		void Initialise(int windowWidth, int windowHeight);
		void Initialise(std::shared_ptr<Graphics::GraphicsContext> graphicsContext, glm::mat4&& projection);

		int GetWidth() { return width; }
		int GetHeight() { return height; }

		glm::mat4 GetView();
		glm::mat4 GetInvertedView();
	private:
		int width;
		int height;

		glm::mat4 projection;
		glm::mat4 invertedProjection;
	};
}