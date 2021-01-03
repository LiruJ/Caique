#pragma once

// Forward declarations.
namespace Graphics { class GraphicsContext; }

namespace GameTiming
{
	// Forward declarations.
	struct GameTime;

	class GameTimeManager
	{
	public:
		GameTimeManager() : targetDeltaSeconds(1.0 / 60.0), frameRateSmoothing(0.8), currentUpdateSeconds(0), lastUpdateSeconds(0), currentDeltaSeconds(0.0), lastDeltaSeconds(0.0) {}

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