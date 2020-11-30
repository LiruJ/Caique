#include "VertexBufferObject.h"

// Graphics includes.
#include <glew.h>

Graphics::VertexBufferObject::VertexBufferObject(const int vertexSize) : bufferData(std::vector<float>()), vertexSize(vertexSize), isDirty(false)
{
	glGenBuffers(1, &graphicsDeviceID);
}

Graphics::VertexBufferObject::VertexBufferObject(const int vertexSize, const std::vector<float>& data) : vertexSize(vertexSize)
{
	glGenBuffers(1, &graphicsDeviceID);
	Add(data);
}

Graphics::VertexBufferObject::~VertexBufferObject() { glDeleteBuffers(1, &graphicsDeviceID); }

void Graphics::VertexBufferObject::Add(const float vertex)
{
	bufferData.push_back(vertex);

	isDirty = true;
}

void Graphics::VertexBufferObject::Add(const glm::vec2 vec2)
{
	bufferData.push_back(vec2.x);
	bufferData.push_back(vec2.y);

	isDirty = true;
}

void Graphics::VertexBufferObject::Add(const glm::vec3 vec3)
{
	bufferData.push_back(vec3.x);
	bufferData.push_back(vec3.y);
	bufferData.push_back(vec3.z);

	isDirty = true;
}

void Graphics::VertexBufferObject::Add(const glm::vec4 vec4)
{
	bufferData.push_back(vec4.x);
	bufferData.push_back(vec4.y);
	bufferData.push_back(vec4.z);
	bufferData.push_back(vec4.w);

	isDirty = true;
}

void Graphics::VertexBufferObject::Add(const std::vector<float>& data)
{
	// Add each vertex in the given vector to the buffer data.
	for (int i = 0; i < data.size(); i++)
		bufferData.push_back(data[i]);
	
	// Since the data has changed within the RAM, mark it as dirty.
	isDirty = true;
}

void Graphics::VertexBufferObject::SetCurrent()
{
	// If the data is dirty, it needs to be reuploaded to the GPU.
	if (isDirty)
	{
		// Begin working on the buffer.
		glBindBuffer(GL_ARRAY_BUFFER, graphicsDeviceID);

		// Upload the data to the GPU.
		glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), &bufferData.at(0), GL_STATIC_DRAW);

		// Since the data has been uploaded, the data is now clean.
		isDirty = false;

		// Return, since the data has been uploaded and this buffer is currently bound.
		return;
	}

	// Start working on this buffer.
	glBindBuffer(GL_ARRAY_BUFFER, graphicsDeviceID);
}

void Graphics::VertexBufferObject::ResetCurrent()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}