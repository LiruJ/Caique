#include "Transform.h"

// GameObject includes.
#include "GameObject.h"

GameObjects::Transform::Transform(std::weak_ptr<GameObjects::GameObject> owningGameObject)
{
	gameObject = owningGameObject;

	//localMatrix = glm::identity<glm::mat4>();
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::identity<glm::quat>();
	//rotation = glm::rotate(rotation, glm::radians(5.0f), glm::vec3(0, 1, 0));
	
}

glm::mat4 GameObjects::Transform::GetLocalMatrix()
{
	if (isLocalDirty)
	{
		localMatrix = glm::identity<glm::mat4>();

		localMatrix = glm::translate(localMatrix, position);
		localMatrix = glm::scale(localMatrix, scale);
		localMatrix *= glm::mat4_cast(rotation);

		isLocalDirty = false;
	}

	return localMatrix;
}

glm::mat4 GameObjects::Transform::GetGlobalMatrix()
{
	// Get self reference and copy shared pointer to currentTransform.



	
	if (isGlobalDirty)
	{

		// Get the parent reference.
		std::shared_ptr<GameObjects::Transform> parentTransform = parent.lock();

		if (parentTransform != nullptr) globalMatrix = parentTransform->GetGlobalMatrix() * GetLocalMatrix();
		else globalMatrix = GetLocalMatrix();

		isGlobalDirty = false;
	}

	return globalMatrix;

	//// Follow the parents upwards until the root matrix is found, then begin calculating from there.
	//std::shared_ptr<GameObjects::Transform> currentTransform = selfReference;
	//glm::mat4 currentGlobalMatrix = glm::identity<glm::mat4>();
	//do
	//{
	//	currentGlobalMatrix = currentTransform->GetLocalMatrix() * currentGlobalMatrix;
	//	currentTransform = currentTransform->parent.lock();
	//}
	//while (currentTransform != nullptr);

	//return currentGlobalMatrix;
}

std::shared_ptr<GameObjects::GameObject> GameObjects::Transform::GetGameObject() { return gameObject.lock(); }

glm::vec3 GameObjects::Transform::GetLocalPosition() { return position; }

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
	for (int i = 0; i < children.size(); i++)
		children.at(i)->dirtyGlobal();
}
