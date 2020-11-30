#pragma once

// Type includes.
#include <string>
#include <vector>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"

namespace Graphics
{
	class GraphicsContext
	{
	public:
		GraphicsContext() {}

		void Initialise();

		void Clear(const float red, const float green, const float blue) const;

		void Draw(const ShaderProgram& shader, const std::vector<unsigned int>& indices, const VertexArrayObject& object) const;
	private:

	};
}