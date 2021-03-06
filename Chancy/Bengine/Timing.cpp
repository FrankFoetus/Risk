#include "Timing.h"

#include <SDL/SDL.h>


namespace Bengine {


	FpsLimiter::FpsLimiter() {

	}

	void FpsLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
	}

	void FpsLimiter::setMaxFPS(float maxFPS) {
		_maxFPS = maxFPS;
	}

	void FpsLimiter::begin() {
		_startTicks = SDL_GetTicks();
	}

	// will return the current FPS
	float FpsLimiter::end() {

		calculateFPS();

		Uint32 frameTicks = SDL_GetTicks() - _startTicks;
		// limit the FPD to the max FPS
		if (1000 / _maxFPS > frameTicks) {
			SDL_Delay(1000 / _maxFPS - frameTicks);
		}

		return _fps;

	}

	void FpsLimiter::calculateFPS(){
		static const Uint32 NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static Uint32 currentFrame = 0;

		static float previousTicks = SDL_GetTicks();
		float currentTicks;

		currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - previousTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		previousTicks = currentTicks;

		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0) {
			_fps = 1000.0f / frameTimeAverage;
		}
		else {
			_fps = 60.0f;
		}
	}

}
