#pragma once

namespace Bengine {

	class FpsLimiter {
	public:
		FpsLimiter();
		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();

		// will return the current FPS
		float end();
	private:
		unsigned int _startTicks;

		float _maxFPS;
		float _fps;
		float _frameTime;

		void calculateFPS();
	};

}