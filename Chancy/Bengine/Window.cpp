#include "Window.h"
#include "BengineError.h"


namespace Bengine {

	Window::Window() : _windowWidth(0),
						_windowHeight(0)
	{
	}


	Window::~Window()
	{
	}


	int Window::create(std::string windowName, int windowWidth, int windowHeight, unsigned int currentFlags) {

		_windowWidth = windowWidth;
		_windowHeight = windowHeight;

		Uint32 flags = SDL_WINDOW_OPENGL;

		// check for passed flags
		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_OPENGL);
		if (_sdlWindow == NULL) {
			fatalError("SDL Window could not be created!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == NULL) {
			fatalError("SDL_GL context could not be created!");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initiliaze glew!");
		}

		glClearColor(72.0f/255.0f, 118.0f/255.0f, 255.0f/255.0f, 1.0f);

		// Check open GL version
		std::printf("*** Open GL Version: %s ***\n", glGetString(GL_VERSION));

		// turn on VSYNC
		SDL_GL_SetSwapInterval(1);

		// enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}


	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}

}