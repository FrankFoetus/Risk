#include <SDL\SDL.h>

#include "Bengine.h"

namespace Bengine {

	// initiliaze Bengine
	extern int init() {

		// initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// tell SDL we want a double buffered window 
		// so we dont get any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// rdy
		return 0;
	}

}