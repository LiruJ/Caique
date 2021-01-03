#pragma once

// Type includes.
#include <memory>
#include <vector>

// Foward declarations.
namespace Content { class ContentManager; }
namespace Behaviours { class Camera; }
namespace GameTiming { struct GameTime; }

namespace GameObjects
{
	class GameObject;
	class Scene : public std::enable_shared_from_this<GameObjects::Scene>
	{
	public:
		static std::shared_ptr<GameObjects::Scene> CreateScene(std::weak_ptr<Content::ContentManager> contentManager);

		std::shared_ptr<GameObjects::GameObject> CreateGameObject();

		void Update(GameTiming::GameTime& gameTime);
		void Draw(Behaviours::Camera& camera);
	private:
		Scene(std::weak_ptr<Content::ContentManager> contentManager) : contentManager(contentManager) {}

		std::weak_ptr<Content::ContentManager> contentManager;

		std::vector<std::shared_ptr<GameObjects::GameObject>> gameObjects;
	};
}