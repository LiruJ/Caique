#include "GraphicsContext.h"

// Graphics includes.
#include <glew.h>

void Graphics::GraphicsContext::Initialise()
{
	// Attempt to initialise glew.
	if (glewInit() != GLEW_OK)
		throw std::exception("Glew failed to initialise.");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Graphics::GraphicsContext::Clear(const float red, const float green, const float blue) const
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::GraphicsContext::Draw(const ShaderProgram& shader, const std::vector<unsigned int>& indices, const VertexArrayObject& object) const
{
	// Use the shader.
	shader.SetCurrent();

	// Use the given object.
	object.SetCurrent();

	// Draw the given object.
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices.at(0));

	// Stop using the given object.
	object.ResetCurrent();

	// Stop using the shader.
	shader.ResetCurrent();
}