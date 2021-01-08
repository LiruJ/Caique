#pragma once

// Graphics includes.
#include <GraphicsContext.h>

// SDL includes.
#include <SDL.h>

// Type includes.
#include <memory>
#include <unordered_set>

// Interface includes.
#include "InputManager.h"

// Forward declarations.
namespace Events { class SDLEventManager; }

namespace Input
{
	class SDLInputManager : public InputManager
	{
		// Mark this class as a friend to the event manager.
		friend class Events::SDLEventManager;
	public:
		SDLInputManager(std::shared_ptr<Graphics::GraphicsContext> graphicsContext) : graphicsContext(graphicsContext), mouseDeltaX(0), mouseDeltaY(0) 
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_SetWindowGrab(graphicsContext->GetWindow(), SDL_TRUE);
		}

		virtual bool GetKeyDown(const char* keyName) { return (IsKeyDown(keyName) && WasKeyUp(keyName)); }
		virtual bool GetKeyUp(const char* keyName) { return (IsKeyUp(keyName) && WasKeyDown(keyName)); }

		virtual bool IsKeyDown(const char* keyName);
		virtual bool IsKeyUp(const char* keyName);
		virtual bool WasKeyDown(const char* keyName);
		virtual bool WasKeyUp(const char* keyName);

		virtual int GetMouseDeltaX() { return mouseDeltaX; }
		virtual int GetMouseDeltaY() { return mouseDeltaY; }
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