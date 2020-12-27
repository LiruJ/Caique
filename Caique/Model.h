#pragma once

// glm includes.
#include <glm/mat4x4.hpp>

// Type includes.
#include <vector>
#include <memory>

namespace Graphics
{
	// Forward declarations.
	class Mesh;

	class Model
	{
	public:
		Model(std::vector<std::shared_ptr<Mesh>> meshes) : meshes(meshes) {}
		void Draw(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix);
	private:
		std::vector<std::shared_ptr<Mesh>> meshes;
	};
}