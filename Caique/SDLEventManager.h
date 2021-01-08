#pragma once

// Type includes.
#include <memory>

// Graphics includes.
#include <GraphicsContext.h>

// Interface includes.
#include "EventManager.h"

// Forward declarations.
namespace Input { class SDLInputManager; }

namespace Events
{
	class SDLEventManager : public EventManager
	{
	public:
		SDLEventManager(std::shared_ptr<Input::SDLInputManager> inputManager, std::shared_ptr<Graphics::GraphicsContext> graphicsContext) : inputManager(inputManager), graphicsContext(graphicsContext), isQuitting(false) {}

		virtual bool IsQuitting() { return isQuitting; }

		virtual void Update();
	private:
		std::weak_ptr<Input::SDLInputManager> inputManager;
		std::shared_ptr<Graphics::GraphicsContext> graphicsContext;

		bool isQuitting;
	};
}