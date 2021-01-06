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
		case SDL_MOUSEMOTION:
			inputManager.lock()->mouseDeltaX += sdlEvent.motion.xrel;
			inputManager.lock()->mouseDeltaY += sdlEvent.motion.yrel;
			break;
		case SDL_WINDOWEVENT:
		{
			switch (sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				graphicsContext->SetOutputSize(sdlEvent.window.data1, sdlEvent.window.data2);
				break;
			}
			break;
		}
		}
	}
}