#include "InputManager.h"

// Event includes.
#include "EventManager.h"

bool Input::InputManager::IsKeyDown(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return currentKeysDown.count(keycode) > 0;
}

bool Input::InputManager::IsKeyUp(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return currentKeysDown.count(keycode) == 0;
}

bool Input::InputManager::WasKeyDown(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return lastKeysDown.count(keycode) > 0;
}

bool Input::InputManager::WasKeyUp(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return lastKeysDown.count(keycode) == 0;
}

void Input::InputManager::setKeyDown(SDL_Keycode keycode)
{
	currentKeysDown.emplace(keycode);
}

void Input::InputManager::setKeyUp(SDL_Keycode keycode)
{
	currentKeysDown.erase(keycode); 
}

void Input::InputManager::refreshState()
{
	// Copy over the current keys to the last keys.
	lastKeysDown = currentKeysDown;

	mouseDeltaX = 0;
	mouseDeltaY = 0;
}
