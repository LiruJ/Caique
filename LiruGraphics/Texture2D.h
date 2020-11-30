#pragma once

namespace Graphics
{
	class Texture2D
	{
	public:
		Texture2D(const int width, const int height);
		Texture2D(const bool isClosest, const int width, const int height, const unsigned char* data);
		~Texture2D();

		void SetCurrent(const int index);

		static void ResetCurrent(const int index);

		int GetWidth() { return width; }

		int GetHeight() { return height; }
	private:
		// The ID of the texture on the GPU.
		unsigned int graphicsDeviceID;

		int width;

		int height;
	
		void initialise();
	};
}