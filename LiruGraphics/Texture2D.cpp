#include "Texture2D.h"

// Glew includes.
#include <glew.h>

// Error handling includes.
#include <exception>

Graphics::Texture2D::Texture2D(const int width, const int height) : width(width), height(height)
{
	initialise();
}

Graphics::Texture2D::Texture2D(const bool isClosest, const int width, const int height, const unsigned char* data) : width(width), height(height)
{
	initialise();

	// Start working on this texture in the GPU.
	glBindTexture(GL_TEXTURE_2D, graphicsDeviceID);

	// Upload the raw image data to the GPU.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Generate the mipmap.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set the filter mode.
	GLenum glFilterMode = (isClosest) ? GL_NEAREST : GL_LINEAR;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilterMode);

	// Stop working on the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
}

Graphics::Texture2D::~Texture2D()
{
	// Delete the texture from the GPU.
	glDeleteTextures(1, &graphicsDeviceID);
}

void Graphics::Texture2D::SetCurrent(const int index)
{
	if (index > 15) return;
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, graphicsDeviceID);
}

void Graphics::Texture2D::ResetCurrent(const int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Texture2D::initialise()
{
	// Create a new texture on the GPU and save the ID.
	glGenTextures(1, &graphicsDeviceID);

	// If the creation failed, throw an exception.
	if (!graphicsDeviceID)
		throw std::exception("Texture could not be created.");
}
