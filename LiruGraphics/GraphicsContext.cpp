#include "GraphicsContext.h"

// Graphics includes.
#include <glew.h>
#include "ShaderProgram.h"
#include "VertexArrayObject.h"

Graphics::GraphicsContext::~GraphicsContext()
{
	// Destroy the context.
	SDL_GL_DeleteContext(context);
	context = 0;

	// Destroy and deference the window.
	SDL_DestroyWindow(window);
	window = nullptr;
}

void Graphics::GraphicsContext::Initialise(int windowWidth, int windowHeight)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::exception("SDL could not be initialised.");

	window = SDL_CreateWindow("Three Entire Dimensions for the Price of Two",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Set up the GL context.
	context = SDL_GL_CreateContext(window);
	if (!context)
		throw std::exception("OpenGL SDL linked incorrectly.");

	// Attempt to initialise glew.
	if (glewInit() != GLEW_OK)
		throw std::exception("Glew failed to initialise.");

	// Enable some default settings.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Set the width and height.
	SDL_GL_GetDrawableSize(window, &outputWidth, &outputHeight);
}

void Graphics::GraphicsContext::SetWindowTitle(std::string& title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

void Graphics::GraphicsContext::SetVSyncMode(VSyncMode vSyncMode)
{
	switch (vSyncMode)
	{
	case Graphics::VSyncMode::None:
		SDL_GL_SetSwapInterval(0);
		break;
	case Graphics::VSyncMode::VSync:
		SDL_GL_SetSwapInterval(1);
		break;
	case Graphics::VSyncMode::AdaptiveVsync:
		if (SDL_GL_SetSwapInterval(-1) == -1) SDL_GL_SetSwapInterval(1);
		break;
	}
}

Graphics::VSyncMode Graphics::GraphicsContext::GetVSyncMode()
{
	int swapInterval = SDL_GL_GetSwapInterval();

	switch (swapInterval)
	{
	case -1:
		return VSyncMode::AdaptiveVsync;
	case 0:
		return VSyncMode::None;
	case 1:
	default:
		return VSyncMode::VSync;
	}

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

void Graphics::GraphicsContext::Present()
{
	SDL_GL_SwapWindow(window);
}