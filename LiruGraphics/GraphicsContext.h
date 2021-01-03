#pragma once

// Type includes.
#include <string>
#include <vector>
#include <memory>

// SDL includes.
#include <SDL.h>

namespace Graphics
{
	enum class VSyncMode { None = 0, VSync, AdaptiveVsync };

	// Forward declarations.
	class VertexArrayObject;
	class ShaderProgram;

	class GraphicsContext
	{
	public:
		GraphicsContext() : outputWidth(0), outputHeight(0), context(0), window(nullptr) {}
		~GraphicsContext();

		void Initialise(int windowWidth, int windowHeight);

		void SetOutputWidth(const int newWidth);
		int GetOutputWidth() { return outputWidth; }
		void SetOutputHeight(const int newHeight);
		int GetOutputHeight() { return outputHeight; }

		void SetWindowTitle(std::string& title) { SetWindowTitle(title.c_str()); }
		void SetWindowTitle(const char* title);
		std::string GetWindowTitle();

		void SetVSyncMode(VSyncMode vSyncMode);

		VSyncMode GetVSyncMode();

		void Clear(const float red, const float green, const float blue) const;

		void Draw(const ShaderProgram& shader, const std::vector<unsigned int>& indices, const VertexArrayObject& object) const;

		void Present();
	private:
		int outputWidth;
		int outputHeight;

		SDL_Window* window;

		SDL_GLContext context;
	};
}