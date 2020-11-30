#pragma once

#include "Behaviour.h"

// GameObject includes.
#include "GameObject.h"
#include "Transform.h"

// GLM includes.
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Behaviours
{
	class Camera : public Behaviour
	{
	public:
		glm::mat4 GetProjection() { return projection; }
		void SetProjection(glm::mat4 projection) { this->projection = projection; }

		glm::mat4 GetView() { return glm::inverse(GetGameObject()->GetTransform()->GetGlobalMatrix()); }
	private:
		glm::mat4 projection;
	};
}