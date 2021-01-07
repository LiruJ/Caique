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
		GameTimeManager();

		void Update();

		const GameTime GetCurrentGameTime();

		const int GetRoughStartID() { return roughStartID; }

		void SetTargetFrameRate(int frameRate) { targetDeltaSeconds = 1.0 / frameRate; }

		const double GetTargetFrameRate() { return 1.0 / targetDeltaSeconds; }

		const int GetCurrentFrameRate();

		void WaitFrameRemainder(Graphics::GraphicsContext& graphicsContext);
	private:
		int roughStartID;

		double targetDeltaSeconds;

		double frameRateSmoothing;

		double currentUpdateSeconds;
		double lastUpdateSeconds;

		double currentDeltaSeconds;
		double lastDeltaSeconds;
	};
}