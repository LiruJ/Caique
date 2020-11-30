#pragma once

namespace Physics
{
	float CalculateVelocity(const float deltaPoint, const float time) 
		{ return (time > 0) ? deltaPoint / time : 0; }

	float CalculateVelocity(const float firstPoint, const float secondPoint, const float time)
		{ return CalculateVelocity(secondPoint - firstPoint, time); }

	float CalculateAcceleration(const float deltaVelocity, const float time)
		{ return (time > 0) ? deltaVelocity / time : 0; }

	float CalculateAcceleration(const float firstVelocity, const float secondVelocity, const float time)
		{ return CalculateAcceleration(secondVelocity - firstVelocity, time); }
}