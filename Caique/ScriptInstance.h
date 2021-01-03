#pragma once

#include "Behaviour.h"

// Type includes.
#include <string>
#include <memory>

namespace Lua { class LuaScript; }

namespace Behaviours
{
	class ScriptInstance : public Behaviour
	{
	public:
		void Initialise() {}
		void Initialise(const std::string& scriptAsset);
		void PostInitialise() override;

		void Update(GameTiming::GameTime& gameTime) override;
		void PostUpdate() override;

		void Draw(Behaviours::Camera& camera) override;

	private:
		std::shared_ptr<Lua::LuaScript> script;
	};
}