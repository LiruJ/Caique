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

		bool SetUniform(const std::string& name, const int value);
		bool SetUniform(const std::string& name, const float value);
		bool SetUniform(const std::string& name, const glm::vec2 value);
		bool SetUniform(const std::string& name, const glm::vec3 value);
		bool SetUniform(const std::string& name, const glm::vec4 value);
		bool SetUniform(const std::string& name, const glm::mat4 value);

		bool SetModelMatrixUniform(const glm::mat4 value) { return SetUniform(modelMatrixName, value); }
		bool SetProjectionMatrixUniform(const glm::mat4 value) { return SetUniform(projectionMatrixName, value); }
		bool SetViewMatrixUniform(const glm::mat4 value) { return SetUniform(viewMatrixName, value); }

		bool SetTextureUniform(const char index) { return SetUniform(texturesName + std::to_string(index), index); }

		bool SetColourUniform(const char index, const glm::vec4 colour) { return SetUniform(coloursName + std::to_string(index), colour); }

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

		bool getUniformLocation(const std::string& name, int& uniformID);
	};
}