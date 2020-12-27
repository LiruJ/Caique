#pragma once

// Type includes.
#include <map>
#include <string>
#include <memory>

namespace Graphics
{
	// Forward declarations.
	class VertexBufferObject;

	class VertexArrayObject
	{
	public:
		
		VertexArrayObject();
		~VertexArrayObject();

		void AddVertexBufferObject(const std::string& name, const unsigned int index, std::shared_ptr<Graphics::VertexBufferObject> value);

		void GetVertexBufferObject(std::shared_ptr<Graphics::VertexBufferObject>& vertexBufferObject, const std::string& name);

		void SetCurrent() const;

		static void ResetCurrent();
	private:
		// The ID of this VAO within the GPU.
		unsigned int graphicsDeviceID;

		std::map<std::string, std::shared_ptr<Graphics::VertexBufferObject>> vertexBufferObjectsByName;
	};
}


