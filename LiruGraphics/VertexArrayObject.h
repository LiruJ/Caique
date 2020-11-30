#pragma once

// Type includes.
#include <map>
#include <string>
#include <memory>

// Graphics includes.
#include "VertexBufferObject.h"

namespace Graphics
{
	class VertexArrayObject
	{
	public:
		
		VertexArrayObject();
		~VertexArrayObject();

		void AddVertexBufferObject(const std::string& name, const unsigned int index, std::shared_ptr<VertexBufferObject> value);

		void GetVertexBufferObject(std::shared_ptr<VertexBufferObject>& vertexBufferObject, const std::string& name);

		void SetCurrent() const;

		static void ResetCurrent();
	private:
		// The ID of this VAO within the GPU.
		unsigned int graphicsDeviceID;

		std::map<std::string, std::shared_ptr<VertexBufferObject>> vertexBufferObjectsByName;
	};
}


