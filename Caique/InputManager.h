#pragma once

// Graphics includes.
#include <GraphicsContext.h>

// SDL includes.
#include <SDL.h>

// Type includes.
#include <memory>
#include <unordered_set>

// Forward declarations.
namespace Events { class EventManager; }

namespace Input
{
	class InputManager
	{
		// Mark this class as a friend to the event manager.
		friend class Events::EventManager;
	public:
		InputManager(std::shared_ptr<Graphics::GraphicsContext> graphicsContext) : graphicsContext(graphicsContext), mouseDeltaX(0), mouseDeltaY(0) 
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_SetWindowGrab(graphicsContext->GetWindow(), SDL_TRUE);
		}

		bool GetKeyDown(const char* keyName) { return (IsKeyDown(keyName) && WasKeyUp(keyName)); }
		bool GetKeyUp(const char* keyName) { return (IsKeyUp(keyName) && WasKeyDown(keyName)); }

		bool IsKeyDown(const char* keyName);
		bool IsKeyUp(const char* keyName);
		bool WasKeyDown(const char* keyName);
		bool WasKeyUp(const char* keyName);

		int GetMouseDeltaX() { return mouseDeltaX; } 
		int GetMouseDeltaY() { return mouseDeltaY; }
	private:
		std::shared_ptr<Graphics::GraphicsContext> graphicsContext;

		std::unordered_set<SDL_Keycode> currentKeysDown;
		std::unordered_set<SDL_Keycode> lastKeysDown;

		int mouseDeltaX;
		int mouseDeltaY;

		void setKeyDown(SDL_Keycode keycode);
		void setKeyUp(SDL_Keycode keycode);



		void refreshState();
	};
}