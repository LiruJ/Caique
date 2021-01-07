#pragma once

// Type includes.
#include <memory>

// Foward declarations.
namespace GameObjects { class GameObject; }
namespace Content { class ContentManager; }
namespace GameTiming { struct GameTime; }

namespace Behaviours
{
	class Camera;
	class Behaviour
	{
	public:
		Behaviour(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::ContentManager> contentManager) : gameObject(gameObject), contentManager(contentManager) {}

		virtual void PreInitialise() {}
		virtual void Initialise() {}
		virtual void PostInitialise() {}

		virtual void Update(GameTiming::GameTime& gameTime) {}
		virtual void PostUpdate() {}

		virtual void Draw(Behaviours::Camera& camera) {}

		std::shared_ptr<GameObjects::GameObject> GetGameObject() { return gameObject.lock(); }
	protected:
		std::weak_ptr<GameObjects::GameObject> gameObject;
		
		std::weak_ptr<Content::ContentManager> contentManager;
	};
}