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
		virtual ~GameTimeManager() {}

		virtual void Update() = 0;

		virtual const GameTime GetCurrentGameTime() = 0;

		virtual void SetTargetFrameRate(int frameRate) = 0;

		virtual const double GetTargetFrameRate() = 0;

		virtual const int GetCurrentFrameRate() = 0;

		virtual void WaitFrameRemainder(Graphics::GraphicsContext& graphicsContext) = 0;
	};
}