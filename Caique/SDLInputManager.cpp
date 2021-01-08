#include "SDLInputManager.h"

// Event includes.
#include "EventManager.h"

bool Input::SDLInputManager::IsKeyDown(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return currentKeysDown.count(keycode) > 0;
}

bool Input::SDLInputManager::IsKeyUp(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return currentKeysDown.count(keycode) == 0;
}

bool Input::SDLInputManager::WasKeyDown(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return lastKeysDown.count(keycode) > 0;
}

bool Input::SDLInputManager::WasKeyUp(const char* keyName)
{
	SDL_Keycode keycode = SDL_GetKeyFromName(keyName);
	return lastKeysDown.count(keycode) == 0;
}

void Input::SDLInputManager::setKeyDown(SDL_Keycode keycode)
{
	currentKeysDown.emplace(keycode);
}

void Input::SDLInputManager::setKeyUp(SDL_Keycode keycode)
{
	currentKeysDown.erase(keycode); 
}

void Input::SDLInputManager::refreshState()
{
	// Copy over the current keys to the last keys.
	lastKeysDown = currentKeysDown;

	mouseDeltaX = 0;
	mouseDeltaY = 0;
}