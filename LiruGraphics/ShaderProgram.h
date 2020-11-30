#pragma once

// Type includes.
#include <vector>
#include <map>
#include <string>
#include <memory>

// glm includes.
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Graphics
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexSource, const std::string& fragmentSource, const std::string& texturesName, const std::string& coloursName, const std::string& modelMatrixName,
			const std::string& projectionMatrixName, const std::string& viewMatrixName);
		~ShaderProgram();

		void Link();

		void SetUniform(const std::string& name, const int value);
		void SetUniform(const std::string& name, const float value);
		void SetUniform(const std::string& name, const glm::vec2 value);
		void SetUniform(const std::string& name, const glm::vec3 value);
		void SetUniform(const std::string& name, const glm::vec4 value);
		void SetUniform(const std::string& name, const glm::mat4 value);

		void SetModelMatrixUniform(const glm::mat4 value) { SetUniform(modelMatrixName, value); }
		void SetProjectionMatrixUniform(const glm::mat4 value) { SetUniform(projectionMatrixName, value); }
		void SetViewMatrixUniform(const glm::mat4 value) { SetUniform(viewMatrixName, value); }

		void SetTextureUniform(const char index) { SetUniform(texturesName + std::to_string(index), index); }

		void SetColourUniform(const char index, const glm::vec4 colour) { SetUniform(coloursName + std::to_string(index), colour); }

		// Sets this program as the current via glUseProgram.
		void SetCurrent() const;

		static void ResetCurrent();
	private:
		// The ID of the shader program on the GPU.
		unsigned int graphicsDeviceID;

		std::vector<unsigned int> unlinkedShaders;

		// A version of the uniforms cached in RAM to save on expensive GPU calls.
		std::map<std::string, int> uniforms;

		std::string modelMatrixName;
		
		std::string projectionMatrixName;
		
		std::string viewMatrixName;
		
		std::string texturesName;

		std::string coloursName;

		void addShader(const int type, const std::string& source);

		int getUniformLocation(const std::string& name);
	};
}