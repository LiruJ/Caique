#pragma once

// Type includes.
#include <memory>

// Foward declarations.
namespace GameObjects { class GameObject; }
namespace Content { class ContentManager; }

namespace Behaviours
{
	class Camera;
	class Behaviour
	{
	public:		
		void Setup(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::ContentManager> contentManager);

		virtual void PreInitialise() {}
		void Initialise() {}
		virtual void PostInitialise() {}

		virtual void Update() {}
		virtual void PostUpdate() {}

		virtual void Draw(Behaviours::Camera& camera) {}

		std::shared_ptr<GameObjects::GameObject> GetGameObject() { return gameObject.lock(); }
	protected:
		std::weak_ptr<GameObjects::GameObject> gameObject;
		
		std::weak_ptr<Content::ContentManager> contentManager;
	};
}