#pragma once

namespace Events
{
	class EventManager
	{
	public:
		virtual ~EventManager() {}

		virtual bool IsQuitting() = 0;

		virtual void Update() = 0;
	};
}