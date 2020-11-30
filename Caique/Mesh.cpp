#include "Mesh.h"

void Graphics::Mesh::Draw(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix)
{
	// Get a shared pointer to the material.
	std::shared_ptr<Graphics::Material> materialPtr = material.lock();

	// Initialise the shader with the material's data.
	materialPtr->InitialiseShader(modelMatrix, projectionMatrix, viewMatrix);

	// Draw the mesh.
	graphicsContext.lock()->Draw(materialPtr->GetShader(), indices, *vertexArrayObject);

	// Reset the shader to clear the state.
	materialPtr->ResetShader();

}