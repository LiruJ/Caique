#pragma once

// Type includes.
#include <string>
#include <vector>
#include <memory>
#include <functional>

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
		void SetOutputSize(const int newWidth, const int newHeight);

		void SetWindowTitle(std::string& title) { SetWindowTitle(title.c_str()); }
		void SetWindowTitle(const char* title);
		std::string GetWindowTitle();

		void SetVSyncMode(VSyncMode vSyncMode);

		VSyncMode GetVSyncMode();

		SDL_Window* GetWindow() { return window; }

		void ListenForResize(std::function<void(int newWidth, int newHeight)> onResize);

		void Clear(const float red, const float green, const float blue) const;

		void Draw(const ShaderProgram& shader, const std::vector<unsigned int>& indices, const VertexArrayObject& object) const;

		void Present();
	private:
		int outputWidth;
		int outputHeight;

		std::vector<std::function<void(int newWidth, int newHeight)>> resizedEventListeners;

		SDL_Window* window;

		SDL_GLContext context;

		void invokeResizeEvent();
	};
}