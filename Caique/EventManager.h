#pragma once

namespace Events
{
	class EventManager
	{
	public:
		virtual ~EventManager() {}

		/// <summary> Returns true if the game should quit; false otherwise. </summary>
		/// <returns> True if the game should quit; false otherwise. </returns>
		virtual bool IsQuitting() = 0;

		/// <summary> Updates and pumps events. </summary>
		virtual void Update() = 0;
	};
}