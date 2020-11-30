#pragma once

#include "IShape.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Physics
{
	class Cylinder : public IShape
	{
	public:
		static float CalculateVolume(const float radius, const float height)
			{ return (float)M_PI * powf(radius, 2.0f) * height; }

		Cylinder(const float radius, const float height) : radius(radius), height(height) { }

		const float GetVolume() { return CalculateVolume(radius, height); }

		const float GetRadius() { return radius; }

		void SetRadius(float radius) { this->radius = radius; }

		const float GetHeight() { return height; }

		void SetHeight(float height) { this->height = height; }

	private:
		float radius;

		float height;
	};
}