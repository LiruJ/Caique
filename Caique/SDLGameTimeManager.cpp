#include "SDLGameTimeManager.h"

// Utility includes.
#include <math.h>

// SDL includes.
#include <SDL.h>

// Graphical includes.
#include "GraphicsContext.h"

// GameTime includes.
#include "GameTime.h"

GameTiming::SDLGameTimeManager::SDLGameTimeManager() : targetDeltaSeconds(1.0 / 60.0), frameRateSmoothing(0.8), currentUpdateSeconds(0), lastUpdateSeconds(0), currentDeltaSeconds(0.0), lastDeltaSeconds(0.0) { }

void GameTiming::SDLGameTimeManager::Update()
{
	lastUpdateSeconds = currentUpdateSeconds;
	currentUpdateSeconds = SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();

	lastDeltaSeconds = currentDeltaSeconds;
	currentDeltaSeconds = (currentUpdateSeconds - lastUpdateSeconds);
}

const GameTiming::GameTime GameTiming::SDLGameTimeManager::GetCurrentGameTime()
{
	GameTime gameTime;

	gameTime.deltaSeconds = currentDeltaSeconds;
	gameTime.deltaMilliseconds = currentDeltaSeconds * 1000.0;

	gameTime.secondsSinceStart = currentUpdateSeconds; 
	gameTime.millisecondsSinceStart = gameTime.secondsSinceStart * 1000.0;

	return gameTime;
}

const int GameTiming::SDLGameTimeManager::GetCurrentFrameRate()
{
	return (int)floor(1.0 / ((lastDeltaSeconds * frameRateSmoothing) + (currentDeltaSeconds * (1.0 - frameRateSmoothing))));
}

void GameTiming::SDLGameTimeManager::WaitFrameRemainder(Graphics::GraphicsContext& graphicsContext)
{
	// Get the vsync mode of the graphics context.
	Graphics::VSyncMode vSyncMode = graphicsContext.GetVSyncMode();

	// If the mode is some form of vsync, do nothing as frame timing is handled differently.
	if (vSyncMode == Graphics::VSyncMode::AdaptiveVsync || vSyncMode == Graphics::VSyncMode::VSync) return;

	// Calculate the time since the frame started and now.
	double frameTime = (SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency()) - currentUpdateSeconds;

	// Calculate the amount of time to wait.
	double waitTime = targetDeltaSeconds - frameTime;

	// If the wait time is less than or equal to 0, no waiting is required as the game is already struggling to hit the target, so return.
	if (waitTime <= 0.0) return;

	// Otherwise; wait for the time.
	else SDL_Delay(floor(waitTime * 1000.0));
}