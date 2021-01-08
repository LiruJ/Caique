#pragma once

namespace Input
{
	class InputManager
	{
	public:
		virtual ~InputManager() {}

		virtual bool GetKeyDown(const char* keyName) = 0;
		virtual bool GetKeyUp(const char* keyName) = 0;

		virtual bool IsKeyDown(const char* keyName) = 0;
		virtual bool IsKeyUp(const char* keyName) = 0;
		virtual bool WasKeyDown(const char* keyName) = 0;
		virtual bool WasKeyUp(const char* keyName) = 0;

		virtual int GetMouseDeltaX() = 0;
		virtual int GetMouseDeltaY() = 0;
	};
}