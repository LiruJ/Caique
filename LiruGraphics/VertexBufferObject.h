#pragma once

// Type includes.
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Graphics
{
	class VertexBufferObject
	{
	public:
		VertexBufferObject(const int vertexSize);
		VertexBufferObject(const int vertexSize, const std::vector<float>& data);
		~VertexBufferObject();

		void Add(const float vertex);
		
		void Add(const glm::vec2 vec2);
		void Add(const glm::vec3 vec3);
		void Add(const glm::vec4 vec4);

		void Add(const std::vector<float>& data);

		void SetCurrent();
		static void ResetCurrent();

		int GetVertexSize() const { return vertexSize; }
	private:
		unsigned int graphicsDeviceID;

		bool isDirty;

		int vertexSize;

		std::vector<float> bufferData;
	};
}