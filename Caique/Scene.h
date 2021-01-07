#pragma once

// Type includes.
#include <memory>
#include <vector>

// Foward declarations.
namespace Content { class ContentManager; }
namespace Behaviours { class Camera; class MeshRenderer; }
namespace GameTiming { struct GameTime; }

namespace GameObjects
{
	class GameObject;
	class Scene : public std::enable_shared_from_this<GameObjects::Scene>
	{
	public:
		static std::shared_ptr<GameObjects::Scene> CreateScene(std::weak_ptr<Content::ContentManager> contentManager);

		std::shared_ptr<GameObjects::GameObject> CreateNewGameObject();

		std::shared_ptr<GameObjects::GameObject> AddNewGameObject();

		void RegisterCamera(std::shared_ptr<Behaviours::Camera> camera);

		void RegisterDrawable(std::shared_ptr<Behaviours::MeshRenderer> drawableObject);

		bool HasBeenInitialised() { return hasInitialised; }

		void Update(GameTiming::GameTime& gameTime);
		void Draw();

		void Initialise();
	private:
		bool hasInitialised;

		std::weak_ptr<Content::ContentManager> contentManager;

		std::vector<std::shared_ptr<GameObjects::GameObject>> gameObjects;

		std::vector<std::weak_ptr<Behaviours::Camera>> cameras;

		std::vector<std::weak_ptr<Behaviours::MeshRenderer>> drawableObjects;

		Scene(std::weak_ptr<Content::ContentManager> contentManager) : contentManager(contentManager), hasInitialised(false) {}
	};
}