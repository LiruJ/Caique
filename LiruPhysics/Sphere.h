#pragma once

#include "IShape.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Physics
{
	class Sphere : public IShape
	{
	public:
		static float CalculateVolume(const float radius)
			{ return (4.0f * (float)M_PI * powf(radius, 3.0f)) / 3.0f; }
		
		Sphere(const float radius) : radius(radius) { }

		const float GetVolume() { return CalculateVolume(radius); }

		const float GetRadius() { return radius; }

		void SetRadius(float radius) { this->radius = radius; }

		void DoThing();

	private:
		float radius;
	};
}