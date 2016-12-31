#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <string>

namespace Bengine {

	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int windowWidth, int windowHeight, unsigned int currentFlags);
		int getWindowWidth() { return _windowWidth; };
		int getWindowHeight() { return _windowHeight; };

		void swapBuffer();

	private:
		SDL_Window* _sdlWindow;
		int _windowWidth;
		int _windowHeight;
	};
}
