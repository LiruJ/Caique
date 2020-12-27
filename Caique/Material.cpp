#include "Material.h"

// Graphics includes.
#include "ShaderProgram.h"
#include "Texture2D.h"

void Graphics::Material::InitialiseShader(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix)
{
	// Set the matrices.
	shader->SetModelMatrixUniform(modelMatrix);
	shader->SetProjectionMatrixUniform(projectionMatrix);
	shader->SetViewMatrixUniform(viewMatrix);

	// Set each texture to active based on its index.
	for (std::pair<char, std::shared_ptr<Graphics::Texture2D>> textureByKey : textures)
	{
		// Set the uniform in the shader.
		shader->SetTextureUniform(textureByKey.first);

		// Set the texture to active.
		textureByKey.second->SetCurrent(textureByKey.first);
	}

	// Set each colour.
	for (const std::pair<char, glm::vec4>& colourByKey : colours)
		shader->SetColourUniform(colourByKey.first, colourByKey.second);
}

void Graphics::Material::ResetShader()
{
	// Reset each texture.
	for (std::pair<char, std::shared_ptr<Graphics::Texture2D>> textureByKey : textures)
		textureByKey.second->ResetCurrent(textureByKey.first);
}