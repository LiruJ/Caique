#pragma once

namespace GameTiming
{
	struct GameTime
	{
	public:		
		double secondsSinceStart;
		double millisecondsSinceStart;

		double deltaSeconds;
		double deltaMilliseconds;

		double GetFramesPerSecond() { return 1.0 / deltaSeconds; }
	};
}