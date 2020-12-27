#include "Model.h"

// Graphics includes.
#include "Mesh.h"

void Graphics::Model::Draw(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix)
{
	for (int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(modelMatrix, projectionMatrix, viewMatrix);
}
