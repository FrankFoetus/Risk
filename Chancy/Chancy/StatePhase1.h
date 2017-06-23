#pragma once

#include "State.h"

class StatePhase1 : public State
{
public:
	StatePhase1();
	~StatePhase1();

	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
private:
};

