#pragma once

// Type includes.
#include <memory>
#include <vector>

// LiruGraphics includes.
#include <GraphicsContext.h>
#include <VertexArrayObject.h>

// glm includes.
#include <glm/mat4x4.hpp>

namespace Graphics
{
	// Forward declarations.
	class Material;

	class Mesh
	{
	public:
		Mesh(std::weak_ptr<Graphics::GraphicsContext> graphicsContext, std::vector<unsigned int>& indices, std::shared_ptr<VertexArrayObject> vertexArrayObject)
			: graphicsContext(graphicsContext), indices(indices), vertexArrayObject(vertexArrayObject), material(nullptr) {}
		Mesh(std::weak_ptr<Graphics::GraphicsContext> graphicsContext, std::vector<unsigned int>& indices, std::shared_ptr<VertexArrayObject> vertexArrayObject, std::shared_ptr<Graphics::Material> material)
			: graphicsContext(graphicsContext), indices(indices), vertexArrayObject(vertexArrayObject), material(material) {}

		const std::shared_ptr<Graphics::Material> GetMaterial() { return material; }

		void Draw(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix);
	private:
		std::weak_ptr<Graphics::GraphicsContext> graphicsContext;

		std::shared_ptr<VertexArrayObject> vertexArrayObject;

		std::vector<unsigned int> indices;

		std::shared_ptr<Graphics::Material> material;
	};
}