#pragma once

// Type includes.
#include <memory>
#include <map>
#include <string>
#include <typeindex>
#include <type_traits>

// Scene includes.
#include "Scene.h"

// Foward declarations.
namespace Content { class JsonContentManager; }
namespace Behaviours { class Behaviour; class Camera; }
namespace GameTiming { struct GameTime; }

namespace GameObjects
{
	class Transform;

	class GameObject : public std::enable_shared_from_this<GameObjects::GameObject>
	{
	public:
		GameObject(std::weak_ptr<GameObjects::Scene> scene, std::weak_ptr<Content::JsonContentManager> contentManager);
		void InitialiseTransform();

		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			// Get the ID of the type argument.
			std::type_index typeIndex = typeid(T);

			// If the component exists, return the component.
			std::map<std::type_index, std::shared_ptr<Behaviours::Behaviour>>::iterator foundItem = behavioursByTypeIndex.find(typeIndex);
			if (foundItem != behavioursByTypeIndex.end())
				return std::dynamic_pointer_cast<T>(foundItem->second);

			// Return null otherwise, and don't set the component.
			return nullptr;
		}

		template<typename T, typename ... Args>
		std::shared_ptr<T> AddComponent(Args&&... args)
		{
			// Create the behaviour.
			std::shared_ptr<T> behaviour = std::make_shared<T>(weak_from_this(), contentManager, std::forward<Args>(args)...);

			// If the scene has already been initialised, initialise this GameObject immediately.
			if (scene.lock()->HasBeenInitialised())
			{
				behaviour->PreInitialise();
				behaviour->Initialise();
				behaviour->PostInitialise();
			}

			behavioursByTypeIndex.emplace(typeid(T), behaviour);

			return behaviour;
		}

		std::shared_ptr<GameObjects::GameObject> AddNewGameObject();

		void PreInitialise();
		void Initialise();
		void PostInitialise();

		void Update(GameTiming::GameTime& gameTime);
		void PostUpdate();

		std::shared_ptr<GameObjects::Transform> GetTransform();

		std::shared_ptr<GameObjects::Scene> GetScene() { return scene.lock(); }
	private:
		std::map<std::type_index, std::shared_ptr<Behaviours::Behaviour>> behavioursByTypeIndex;

		std::shared_ptr<GameObjects::Transform> transform;

		std::weak_ptr<Content::JsonContentManager> contentManager;

		std::weak_ptr<GameObjects::Scene> scene;
	};
}