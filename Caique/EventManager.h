#pragma once

// Type includes.
#include <memory>

// Graphics includes.
#include <GraphicsContext.h>

// Forward declarations.
namespace Input { class InputManager; }

namespace Events
{
	class EventManager
	{
	public:
		EventManager(std::shared_ptr<Input::InputManager> inputManager, std::shared_ptr<Graphics::GraphicsContext> graphicsContext) : inputManager(inputManager), graphicsContext(graphicsContext), isQuitting(false) {}

		bool IsQuitting() { return isQuitting; }

		void Update();
	private:
		std::weak_ptr<Input::InputManager> inputManager;
		std::shared_ptr<Graphics::GraphicsContext> graphicsContext;

		bool isQuitting;
	};
}