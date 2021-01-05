#pragma once

// SDL includes.
#include <SDL.h>

// Type includes.
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
		bool GetKeyDown(const char* keyName) { return (IsKeyDown(keyName) && WasKeyUp(keyName)); }
		bool GetKeyUp(const char* keyName) { return (IsKeyUp(keyName) && WasKeyDown(keyName)); }

		bool IsKeyDown(const char* keyName);
		bool IsKeyUp(const char* keyName);
		bool WasKeyDown(const char* keyName);
		bool WasKeyUp(const char* keyName);
	private:
		std::unordered_set<SDL_Keycode> currentKeysDown;
		std::unordered_set<SDL_Keycode> lastKeysDown;

		void setKeyDown(SDL_Keycode keycode);
		void setKeyUp(SDL_Keycode keycode);

		void refreshState();
	};
}