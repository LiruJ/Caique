#pragma once

namespace Physics
{
	class IShape
	{
	public:
		virtual const float GetVolume() = 0;
	};
}