#include "VertexArrayObject.h"

// Graphics includes.
#include <glew.h>

Graphics::VertexArrayObject::VertexArrayObject()
{
	// Create the VAO on the GPU and save the ID.
	glGenVertexArrays(1, &graphicsDeviceID);

	// Throw an exception if the ID is invalid.
	if (!graphicsDeviceID)
		throw std::exception("Vertex Array Object could not be created.");
}

Graphics::VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &graphicsDeviceID);
}

void Graphics::VertexArrayObject::AddVertexBufferObject(const std::string& name, const unsigned int index, std::shared_ptr<Graphics::VertexBufferObject> value)
{
	// Start working on this VAO and the given VBO.
	SetCurrent();
	value->SetCurrent();

	// Set and enable the attribute.
	glVertexAttribPointer(index, value->GetVertexSize(), GL_FLOAT, GL_FALSE, sizeof(float) * value->GetVertexSize(), (void*)0);
	glEnableVertexAttribArray(index);

	// Reset the state.
	ResetCurrent();
	value->ResetCurrent();

	// Add the VBO to this VAO's list.
	vertexBufferObjectsByName.emplace(name, value);
}

void Graphics::VertexArrayObject::GetVertexBufferObject(std::shared_ptr<Graphics::VertexBufferObject>& vertexBufferObject, const std::string& name)
{
	// Try to find the VAO within the map.
	std::map<std::string, std::shared_ptr<VertexBufferObject>>::iterator foundItem = vertexBufferObjectsByName.find(name);

	// Return the vbo.
	vertexBufferObject = (*foundItem).second;
}

void Graphics::VertexArrayObject::SetCurrent() const
{
	glBindVertexArray(graphicsDeviceID);
}

void Graphics::VertexArrayObject::ResetCurrent()
{
	VertexBufferObject::ResetCurrent();
	glBindVertexArray(0);
}
