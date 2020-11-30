#pragma once

// Type includes.
#include <memory>
#include <vector>

// GameObject includes.
#include "GameObject.h"

// Content includes.
#include "ContentManager.h"

// Graphical includes.
#include "Camera.h"

namespace GameObjects
{
	class Scene
	{
	public:
		static std::shared_ptr<GameObjects::Scene> CreateScene(std::weak_ptr<Content::ContentManager> contentManager);

		std::shared_ptr<GameObjects::GameObject> CreateGameObject();

		void Draw(Behaviours::Camera& camera);
	private:
		Scene(std::weak_ptr<Content::ContentManager> contentManager) : contentManager(contentManager) {}

		std::weak_ptr<Content::ContentManager> contentManager;

		std::vector<std::shared_ptr<GameObjects::GameObject>> gameObjects;
	};
}