#pragma once

namespace Input
{
	class InputManager
	{
	public:
		virtual ~InputManager() {}

		/// <summary> Finds if the key with the given name is down this frame and up the last. </summary>
		/// <param name="keyName"> The name of the key. </param>
		/// <returns> True if the key with the given name is down this frame and up the last; otherwise, false. </returns>
		virtual bool GetKeyDown(const char* keyName) = 0;

		/// <summary> Finds if the key with the given name is up this frame and down the last. </summary>
		/// <param name="keyName"> The name of the key. </param>
		/// <returns> True if the key with the given name is up this frame and down the last; otherwise, false. </returns>
		virtual bool GetKeyUp(const char* keyName) = 0;

		/// <summary> Finds if the key with the given name is down this frame. </summary>
		/// <param name="keyName"> The name of the key. </param>
		/// <returns> True if the key with the given name is down this frame; otherwise, false. </returns>
		virtual bool IsKeyDown(const char* keyName) = 0;

		/// <summary> Finds if the key with the given name is up this frame. </summary>
		/// <param name="keyName"> The name of the key. </param>
		/// <returns> True if the key with the given name is up this frame; otherwise, false. </returns>
		virtual bool IsKeyUp(const char* keyName) = 0;

		/// <summary> Finds if the key with the given name was down last frame. </summary>
		/// <param name="keyName"> The name of the key. </param>
		/// <returns> True if the key with the given name was down last frame; otherwise, false. </returns>
		virtual bool WasKeyDown(const char* keyName) = 0;

		/// <summary> Finds if the key with the given name was up last frame. </summary>
		/// <param name="keyName"> The name of the key. </param>
		/// <returns> True if the key with the given name was up last frame; otherwise, false. </returns>
		virtual bool WasKeyUp(const char* keyName) = 0;

		/// <summary> Gets the x movement of the mouse since the last frame. </summary>
		/// <returns> The x movement of the mouse since the last frame. </returns>
		virtual int GetMouseDeltaX() = 0;

		/// <summary> Gets the y movement of the mouse since the last frame. </summary>
		/// <returns> The y movement of the mouse since the last frame. </returns>
		virtual int GetMouseDeltaY() = 0;
	};
}