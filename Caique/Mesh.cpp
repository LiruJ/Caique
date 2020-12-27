#include "Mesh.h"

// Graphics includes.
#include "Material.h"

void Graphics::Mesh::Draw(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix)
{
	// If the material is null, do nothing.
	if (material == nullptr) return;

	// Initialise the shader with the material's data.
	material->InitialiseShader(modelMatrix, projectionMatrix, viewMatrix);

	// Draw the mesh.
	graphicsContext.lock()->Draw(*material->GetShader().get(), indices, *vertexArrayObject);

	// Reset the shader to clear the state.
	material->ResetShader();

}