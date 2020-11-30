#pragma once

// Type includes.
#include "Mesh.h"

// glm includes.
#include <glm/mat4x4.hpp>

namespace Graphics
{
	class Model
	{
	public:
		Model(std::vector<Mesh> meshes) : meshes(meshes) {}
		void Draw(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix);
	private:
		std::vector<Mesh> meshes;
	};
}