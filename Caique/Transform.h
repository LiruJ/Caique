#pragma once

// Type includes.
#include <memory>
#include <vector>

// GLM includes.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext.hpp>

namespace GameObjects
{
	class GameObject;

	class Transform : public std::enable_shared_from_this<GameObjects::Transform>
	{
	public:
		Transform(std::weak_ptr<GameObjects::GameObject> owningGameObject);
		Transform() {}

		glm::mat4 GetLocalMatrix();

		glm::mat4 GetGlobalMatrix();

		std::shared_ptr<GameObjects::GameObject> GetGameObject();

		glm::vec3 GetLocalPosition();

		void SetLocalPosition(const glm::vec3 position) { this->position = position; isLocalDirty = true; dirtyGlobal(); }

		void AddChild(std::shared_ptr<GameObjects::Transform> child);

		std::shared_ptr<GameObjects::Transform> GetChildByIndex(int index) { return children.at(index); }

		int GetChildCount() { return children.size(); }
	private:
		std::weak_ptr<GameObjects::Transform> parent;

		std::vector<std::shared_ptr<GameObjects::Transform>> children;

		std::weak_ptr<GameObjects::GameObject> gameObject;

		bool isLocalDirty = true;

		bool isGlobalDirty = true;

		glm::mat4 localMatrix;

		glm::vec3 position;

		glm::vec3 scale;

		glm::quat rotation;

		glm::mat4 globalMatrix;

		void dirtyGlobal();
	};
}