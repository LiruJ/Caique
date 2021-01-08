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
		ScriptInstance(std::weak_ptr<GameObjects::GameObject> gameObject, std::weak_ptr<Content::JsonContentManager> contentManager, const std::string& scriptName);

		void PreInitialise() override;
		void Initialise() override;
		void PostInitialise() override;

		void Update(GameTiming::GameTime& gameTime) override;
		void PostUpdate() override;

		const std::shared_ptr<Lua::LuaScript> GetScript() { return script; }

	private:
		std::shared_ptr<Lua::LuaScript> script;
	};
}