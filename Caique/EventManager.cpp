#include "EventManager.h"

// SDL includes.
#include <SDL.h>

// Input includes.
#include "InputManager.h"

void Events::EventManager::Update()
{
	// Refresh the input manager state.
	inputManager.lock()->refreshState();

	SDL_Event sdlEvent = { 0 };

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			isQuitting = true;
			break;
		case SDL_KEYDOWN:
			inputManager.lock()->setKeyDown(SDL_GetKeyFromScancode(sdlEvent.key.keysym.scancode));
			break;
		case SDL_KEYUP:
			inputManager.lock()->setKeyUp(SDL_GetKeyFromScancode(sdlEvent.key.keysym.scancode));
			break;
		}
	}
}