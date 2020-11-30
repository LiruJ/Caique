#include "Material.h"

void Graphics::Material::InitialiseShader(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix)
{
	// Set the matrices.
	std::shared_ptr<Graphics::ShaderProgram> shaderProgram = shader.lock();
	shaderProgram->SetModelMatrixUniform(modelMatrix);
	shaderProgram->SetProjectionMatrixUniform(projectionMatrix);
	shaderProgram->SetViewMatrixUniform(viewMatrix);

	// Set each texture to active based on its index.
	for (std::pair<char, std::weak_ptr<Graphics::Texture2D>> textureByKey : textures)
	{
		// Set the uniform in the shader.
		shaderProgram->SetTextureUniform(textureByKey.first);

		// Set the texture to active.
		textureByKey.second.lock()->SetCurrent(textureByKey.first);
	}

	// Set each colour.
	for (std::pair<char, glm::vec4> colourByKey : colours)
		shaderProgram->SetColourUniform(colourByKey.first, colourByKey.second);
}

void Graphics::Material::ResetShader()
{
	// Reset each texture.
	for (std::pair<char, std::weak_ptr<Graphics::Texture2D>> textureByKey : textures)
		textureByKey.second.lock()->ResetCurrent(textureByKey.first);
}