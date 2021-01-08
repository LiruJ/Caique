#pragma once

// Interface includes.
#include "GameTimeManager.h"

// Forward declarations.
namespace Graphics { class GraphicsContext; }

namespace GameTiming
{
	// Forward declarations.
	struct GameTime;

	class SDLGameTimeManager : public GameTimeManager
	{
	public:
		SDLGameTimeManager();

		void Update();

		const GameTime GetCurrentGameTime();

		void SetTargetFrameRate(int frameRate) { targetDeltaSeconds = 1.0 / frameRate; }

		const double GetTargetFrameRate() { return 1.0 / targetDeltaSeconds; }

		const int GetCurrentFrameRate();

		void WaitFrameRemainder(Graphics::GraphicsContext& graphicsContext);
	private:
		double targetDeltaSeconds;

		double frameRateSmoothing;

		double currentUpdateSeconds;
		double lastUpdateSeconds;

		double currentDeltaSeconds;
		double lastDeltaSeconds;
	};
}