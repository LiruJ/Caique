#pragma once

#include "IShape.h"

namespace Physics
{
	class Pyramid : public IShape
	{
	public:
		static float CalculateVolume(const float base, const float height)
		{ return (base * height) / 3.0f; }

		static float CalculateVolume(const float baseWidth, const float baseDepth, const float height)
		{ return (baseWidth * baseDepth * height) / 3.0f; }

		Pyramid(const float baseWidth, const float baseDepth, const float height) 
			: baseWidth(baseWidth), baseDepth(baseDepth), height(height) {}

		const float GetVolume() { return CalculateVolume(baseWidth, baseDepth, height); }

		const float GetBaseWidth() { return baseWidth; }

		void SetBaseWidth(float baseWidth) { this->baseWidth = baseWidth; }

		const float GetBaseDepth() { return baseDepth; }

		void SetBaseDepth(float baseDepth) { this->baseDepth = baseDepth; }

		const float GetHeight() { return height; }

		void SetHeight(float height) { this->height = height; }
	private:
		float baseWidth;

		float baseDepth;

		float height;
	};
}