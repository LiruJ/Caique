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

		glm::mat4 GetLocalMatrix();

		glm::mat4 GetInvertedLocalMatrix();

		glm::mat4 GetGlobalMatrix();

		glm::mat4 GetInvertedGlobalMatrix();

		std::shared_ptr<GameObjects::GameObject> GetGameObject();

		glm::vec3 GetLocalPosition();

		glm::vec3 GetGlobalPosition();

		glm::quat GetLocalRotation();

		void SetLocalPosition(const glm::vec3 position) { this->position = position; isLocalDirty = true; dirtyGlobal(); }

		void SetLocalRotation(const glm::quat rotation) { this->rotation = rotation; isLocalDirty = true; dirtyGlobal(); }

		void RotateAround(const float angle, glm::vec3 axis);

		void SetLocalScale(const glm::vec3 scale) { this->scale = scale; isLocalDirty = true; dirtyGlobal(); }

		void LookAt(const glm::vec3 target) { this->rotation = glm::quatLookAt(glm::normalize(target - position), glm::vec3(0, 1, 0)); isLocalDirty = true; dirtyGlobal(); }

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

		glm::mat4 invertedLocalMatrix;

		glm::vec3 position;

		glm::vec3 scale;

		glm::quat rotation;

		glm::mat4 globalMatrix;

		glm::mat4 invertedGlobalMatrix;

		void dirtyGlobal();
	};
}