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

		/// <summary> Gets the Behaviour with the given type. </summary>
		/// <typeparam name="T"> The type of Behaviour. </typeparam>
		/// <returns> The Behaviour with the given type. </returns>
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

		/// <summary> Adds a Behaviour of the given type to this GameObject, using the constructor found from the given args. </summary>
		/// <typeparam name="T"> The Behaviour type. </typeparam>
		/// <typeparam name="...Args"> The arguments to be passed through to the Behaviour. </typeparam>
		/// <param name="...args"> The arguments to be passed through to the Behaviour. </param>
		/// <returns> The created Behaviour. </returns>
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

		/// <summary> The first initialisation function, having a top-down effect. </summary>
		void PreInitialise();

		/// <summary> The second initialisation function, having a bottom-up effect. </summary>
		void Initialise();
		
		/// <summary> The last initialisation function, having a top-down effect. </summary>
		void PostInitialise();

		/// <summary> Runs every frame. </summary>
		/// <param name="gameTime"> The current game time. </param>
		void Update(GameTiming::GameTime& gameTime);

		/// <summary> Runs every frame after the main update function. </summary>
		void PostUpdate();

		/// <summary> Gets this GameObject's transform. </summary>
		/// <returns> The transform of this GameObject. </returns>
		std::shared_ptr<GameObjects::Transform> GetTransform();

		/// <summary> Gets the Scene that this GameObject belongs to. </summary>
		/// <returns> The owning scene. </returns>
		std::shared_ptr<GameObjects::Scene> GetScene() { return scene.lock(); }
	private:
		std::map<std::type_index, std::shared_ptr<Behaviours::Behaviour>> behavioursByTypeIndex;

		std::shared_ptr<GameObjects::Transform> transform;

		std::weak_ptr<Content::JsonContentManager> contentManager;

		std::weak_ptr<GameObjects::Scene> scene;
	};
}