#pragma once

#include "IShape.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Physics
{
	class Ellipsoid : public IShape
	{
	public:
		static float CalculateVolume(const float width, const float height, const float depth)
			{ return (4.0f / 3.0f) * (float)M_PI * width * height * depth; }

		Ellipsoid(const float width, const float height, const float depth) : width(width), height(height), depth(depth) { }

		const float GetVolume() { return CalculateVolume(width, depth, height); }

		const float GetWidth() { return width; }

		void SetWidth(float baseWidth) { this->width = baseWidth; }

		const float GetDepth() { return depth; }

		void SetDepth(float baseDepth) { this->depth = baseDepth; }

		const float GetHeight() { return height; }

		void SetHeight(float height) { this->height = height; }

	private:
		float width;

		float height;

		float depth;
	};
}