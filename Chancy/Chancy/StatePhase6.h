#pragma once

#include "State.h"

class StatePhase6 : public State
{
public:
	StatePhase6();
	~StatePhase6();

	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
};

