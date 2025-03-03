#include "ShaderProgram.h"

// Graphics includes.
#include <glew.h>

// Utility includes.
#include "FileReaders.h"
#include <glm/gtc/type_ptr.hpp>

// Stream includes.
#include <sstream>

Graphics::ShaderProgram::ShaderProgram(const std::string& vertexSource, const std::string& fragmentSource, const std::string& texturesName, const std::string& coloursName, const std::string& modelMatrixName,
	const std::string& projectionMatrixName, const std::string& viewMatrixName)
	: texturesName(texturesName), coloursName(coloursName), modelMatrixName(modelMatrixName), projectionMatrixName(projectionMatrixName), viewMatrixName(viewMatrixName)
{
	// Create a new program and save the ID from the GPU.
	graphicsDeviceID = glCreateProgram();

	// Throw an exception if the ID is invalid.
	if (!graphicsDeviceID)
		throw std::exception("Shader program could not be created.");

	// Add the shaders.
	addShader(GL_VERTEX_SHADER, vertexSource);
	addShader(GL_FRAGMENT_SHADER, fragmentSource);
}

Graphics::ShaderProgram::~ShaderProgram() { glDeleteProgram(graphicsDeviceID); }

void Graphics::ShaderProgram::Link()
{
	// Link the shader program.
	glLinkProgram(graphicsDeviceID);

	// Get the link result.
	int linkResult = 0;
	glGetProgramiv(graphicsDeviceID, GL_LINK_STATUS, &linkResult);

	// If the link failed, throw an exception.
	if (!linkResult)
	{
		// Get the error message from the GPU.
		int messageLength = 0;
		char message[1024];
		glGetProgramInfoLog(graphicsDeviceID, 1024, &messageLength, message);

		// Throw an exception with the error message.
		throw std::exception(message);
	}

	// Detach and delete the unlinked shaders.
	for (int i = 0; i < unlinkedShaders.size(); i++)
	{
		glDetachShader(graphicsDeviceID, unlinkedShaders.at(i));
		glDeleteShader(unlinkedShaders.at(i));
	}

	// Remove the IDs of the shaders since they are now all invalid.
	unlinkedShaders.clear();
}

bool Graphics::ShaderProgram::SetUniform(const std::string& name, const int value)
{
	// Start using this shader.
	SetCurrent();

	// Get the uniform location, if it exists.
	int uniformID;
	bool uniformValid = getUniformLocation(name, uniformID);

	// If the uniform exists, set the data.
	if (uniformValid)
	{
		// Set the value of the uniform.
		glUniform1i(uniformID, value);

		// Stop using this shader.
		ResetCurrent();
	}

	// Return the result of the get operation.
	return uniformValid;
}

bool Graphics::ShaderProgram::SetUniform(const std::string& name, const float value)
{
	SetCurrent();

	// Get the uniform location, if it exists.
	int uniformID;
	bool uniformValid = getUniformLocation(name, uniformID);

	// If the uniform exists, set the data.
	if (uniformValid)
	{
		glUniform1f(uniformID, value);
		ResetCurrent();
	}

	// Return the result of the get operation.
	return uniformValid;
}

bool Graphics::ShaderProgram::SetUniform(const std::string& name, const glm::vec2 value)
{
	SetCurrent();

	// Get the uniform location, if it exists.
	int uniformID;
	bool uniformValid = getUniformLocation(name, uniformID);

	// If the uniform exists, set the data.
	if (uniformValid)
	{
		glUniform2f(uniformID, value.x, value.y);
		ResetCurrent();
	}

	// Return the result of the get operation.
	return uniformValid;
}

bool Graphics::ShaderProgram::SetUniform(const std::string& name, const glm::vec3 value)
{
	SetCurrent();

	// Get the uniform location, if it exists.
	int uniformID;
	bool uniformValid = getUniformLocation(name, uniformID);

	// If the uniform exists, set the data.
	if (uniformValid)
	{
		glUniform3f(uniformID, value.x, value.y, value.z);
		ResetCurrent();
	}

	// Return the result of the get operation.
	return uniformValid;
}

bool Graphics::ShaderProgram::SetUniform(const std::string& name, const glm::vec4 value)
{
	SetCurrent();

	// Get the uniform location, if it exists.
	int uniformID;
	bool uniformValid = getUniformLocation(name, uniformID);

	// If the uniform exists, set the data.
	if (uniformValid)
	{
		glUniform4f(uniformID, value.x, value.y, value.z, value.w);
		ResetCurrent();
	}

	// Return the result of the get operation.
	return uniformValid;
}

bool Graphics::ShaderProgram::SetUniform(const std::string& name, const glm::mat4 value)
{
	SetCurrent();
	
	// Get the uniform location, if it exists.
	int uniformID;
	bool uniformValid = getUniformLocation(name, uniformID);

	// If the uniform exists, set the data.
	if (uniformValid)
	{
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(value));
		ResetCurrent();
	}

	// Return the result of the get operation.
	return uniformValid;
}

// Sets this program as the current via glUseProgram.
void Graphics::ShaderProgram::SetCurrent() const
{
	glUseProgram(graphicsDeviceID);
}

void Graphics::ShaderProgram::ResetCurrent()
{
	glUseProgram(0);
}

void Graphics::ShaderProgram::addShader(const int type, const std::string& source)
{
	// Create a new shader on the GPU and save the ID.
	unsigned int shaderID = glCreateShader(type);

	// Throw an exception if the creation failed.
	if (!shaderID)
		throw std::exception("Shader creation failed.");

	// Set the source of the shader.
	const char* sourceCString = source.c_str();
	glShaderSource(shaderID, 1, &sourceCString, NULL);

	// Compile the source on the GPU.
	glCompileShader(shaderID);

	// Check to see that the compilation was successful.
	GLint compilationResult = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationResult);
	if (compilationResult == GL_FALSE)
	{
		// Get the error message from the GPU.
		int messageLength = 0;
		char message[8192];
		glGetShaderInfoLog(shaderID, 8192, &messageLength, message);

		// Throw an exception with the error message.
		throw std::exception(message);
	}

	// Add the ID of the shader to the list of unlinked shaders.
	unlinkedShaders.push_back(shaderID);

	// Attach the shader to the program.
	glAttachShader(graphicsDeviceID, shaderID);
}

bool Graphics::ShaderProgram::getUniformLocation(const std::string& name, int& uniformID)
{
	// Try get the uniform ID from the RAM map.
	std::map<std::string, int>::iterator foundItem = uniforms.find(name);

	// If the uniform was found in the map, return true.
	if (foundItem != uniforms.end())
	{
		uniformID = foundItem->second;
		return true;
	}

	// Otherwise, attempt to get the uniform from the GPU.
	uniformID = glGetUniformLocation(graphicsDeviceID, name.c_str());

	// If the given ID is above -1 (valid), add it to the map and return true.
	if (uniformID > -1)
	{
		uniforms.emplace(name, uniformID);
		return true;
	}

	// Otherwise, return false.
	return false;
}