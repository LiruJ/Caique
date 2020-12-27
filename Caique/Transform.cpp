#include "Transform.h"

// GameObject includes.
#include "GameObject.h"

GameObjects::Transform::Transform(std::weak_ptr<GameObjects::GameObject> owningGameObject)
{
	gameObject = owningGameObject;

	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::identity<glm::quat>();
}

glm::mat4 GameObjects::Transform::GetLocalMatrix()
{
	if (isLocalDirty)
	{
		localMatrix = glm::identity<glm::mat4>();

		localMatrix = glm::translate(localMatrix, position);
		localMatrix = glm::scale(localMatrix, scale);
		localMatrix *= glm::mat4_cast(rotation);
		
		invertedLocalMatrix = glm::inverse(localMatrix);

		isLocalDirty = false;
	}

	return localMatrix;
}

glm::mat4 GameObjects::Transform::GetInvertedLocalMatrix()
{
	if (isLocalDirty) GetLocalMatrix();
	return invertedLocalMatrix;
}

glm::mat4 GameObjects::Transform::GetGlobalMatrix()
{
	if (isGlobalDirty)
	{
		// Get the parent reference.
		std::shared_ptr<GameObjects::Transform> parentTransform = parent.lock();

		if (parentTransform != nullptr) globalMatrix = parentTransform->GetGlobalMatrix() * GetLocalMatrix();
		else globalMatrix = GetLocalMatrix();

		invertedGlobalMatrix = glm::inverse(globalMatrix);

		isGlobalDirty = false;
	}

	return globalMatrix;
}

glm::mat4 GameObjects::Transform::GetInvertedGlobalMatrix()
{
	if (isGlobalDirty) GetGlobalMatrix();
	return invertedGlobalMatrix;
}

std::shared_ptr<GameObjects::GameObject> GameObjects::Transform::GetGameObject() { return gameObject.lock(); }

glm::vec3 GameObjects::Transform::GetLocalPosition() { return position; }

glm::vec3 GameObjects::Transform::GetGlobalPosition()
{
	return glm::vec3(GetInvertedGlobalMatrix() * glm::vec4(GetLocalPosition(), 1));
}

glm::quat GameObjects::Transform::GetLocalRotation()
{
	return rotation;
}

void GameObjects::Transform::RotateAround(const float angle, glm::vec3 axis)
{
	rotation = glm::rotate(rotation, angle, axis);
	isLocalDirty = true;
	dirtyGlobal();
}

void GameObjects::Transform::AddChild(std::shared_ptr<GameObjects::Transform> child)
{
	// TODO: Properly implement this function so it's safe to use.
	children.push_back(child);
	child->parent = weak_from_this();
	child->dirtyGlobal();
}

void GameObjects::Transform::dirtyGlobal()
{
	// Set the dirty of this transform to true.
	isGlobalDirty = true;

	// Do the same for all children.
	for (size_t i = 0; i < children.size(); i++)
		children.at(i)->dirtyGlobal();
}
