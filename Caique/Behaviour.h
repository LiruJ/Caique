#pragma once

// Type includes.
#include <memory>

// Foward declarations.
namespace GameObjects { class GameObject; }
namespace Content { class JsonContentManager; }
namespace GameTiming { struct GameTime; }

namespace Behaviours
{
	class Camera;
	class Behaviour
	{
	public:
		Behaviour(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::JsonContentManager> contentManager) : gameObject(gameObject), contentManager(contentManager) {}

		virtual void PreInitialise() {}
		virtual void Initialise() {}
		virtual void PostInitialise() {}

		virtual void Update(GameTiming::GameTime& gameTime) {}
		virtual void PostUpdate() {}

		std::shared_ptr<GameObjects::GameObject> GetGameObject() { return gameObject.lock(); }
	protected:
		std::weak_ptr<GameObjects::GameObject> gameObject;
		
		std::weak_ptr<Content::JsonContentManager> contentManager;
	};
}