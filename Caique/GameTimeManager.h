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

		/// <summary> Updates the current game time. </summary>
		virtual void Update() = 0;

		/// <summary> Gets the GameTime at the time of the function call. </summary>
		/// <returns> The current GameTime. </returns>
		virtual const GameTime GetCurrentGameTime() = 0;

		/// <summary> Sets the target FPS. Only has an effect is vsync is off. </summary>
		/// <param name="frameRate"> The new target FPS. </param>
		virtual void SetTargetFrameRate(int frameRate) = 0;

		/// <summary> Gets the target FPS, if vsync is off. </summary>
		/// <returns> The target FPS.</returns>
		virtual const double GetTargetFrameRate() = 0;

		/// <summary> Gets the current frames per second, slightly smoothed. </summary>
		/// <returns> The current smoothed FPS. </returns>
		virtual const int GetCurrentFrameRate() = 0;

		/// <summary> Waits for the remainder of the frame, based on the start of the current frame and the time at which this function was called. </summary>
		/// <param name="graphicsContext"> The graphics context, used for timing. </param>
		virtual void WaitFrameRemainder(Graphics::GraphicsContext& graphicsContext) = 0;
	};
}