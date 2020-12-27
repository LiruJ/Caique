#pragma once

// Type includes.
#include <memory>
#include <map>

// glm includes.
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Graphics
{
	// Forward declarations.
	class ShaderProgram;
	class Texture2D;

	class Material
	{
	public:
		Material(std::shared_ptr<Graphics::ShaderProgram> shader, std::map<char, std::shared_ptr<Graphics::Texture2D>>& textures, std::map<char, glm::vec4> colours) : shader(shader), textures(textures), colours(colours) {}

		void InitialiseShader(const glm::mat4 modelMatrix, const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix);
		void ResetShader();

		const std::shared_ptr<Graphics::ShaderProgram> GetShader() { return shader; }

	private:
		std::shared_ptr<Graphics::ShaderProgram> shader;

		std::map<char, std::shared_ptr<Graphics::Texture2D>> textures;

		std::map<char, glm::vec4> colours;
	};
}