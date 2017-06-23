#pragma once

#include "State.h"

class StatePhase5 : public State
{
public:
	StatePhase5();
	~StatePhase5();

	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
};

