#pragma once

// Graphics includes.
#include "Material.h"

// Type includes.
#include <memory>
#include <vector>

// LiruGraphics includes.
#include <GraphicsContext.h>
#include <Texture2D.h>
#include <VertexArrayObject.h>
#include <ShaderProgram.h>

// glm includes.
#include <glm/mat4x4.hpp>

namespace Graphics
{
	class Mesh
	{
	public:
		Mesh(std::weak_ptr<GraphicsContext> graphicsContext, std::vector<unsigned int>& indices, std::shared_ptr<VertexArrayObject> vertexArrayObject, std::weak_ptr<Material> material) 
			: graphicsContext(graphicsContext), indices(indices), vertexArrayObject(vertexArrayObject), material(material) {}

		void Draw(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix);
	private:
		std::weak_ptr<GraphicsContext> graphicsContext;

		std::shared_ptr<VertexArrayObject> vertexArrayObject;

		std::vector<unsigned int> indices;

		std::weak_ptr<Material> material;
	};
}