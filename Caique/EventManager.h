#pragma once

// Type includes.
#include <memory>

// Forward declarations.
namespace Input { class InputManager; }

namespace Events
{
	class EventManager
	{
	public:
		EventManager(std::shared_ptr<Input::InputManager> inputManager) : inputManager(inputManager), isQuitting(false) {}

		bool IsQuitting() { return isQuitting; }

		void Update();
	private:
		std::weak_ptr<Input::InputManager> inputManager;

		bool isQuitting;
	};
}