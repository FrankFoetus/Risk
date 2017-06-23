#pragma once

#include "State.h"

class StatePhase4 : public State
{
public:
	StatePhase4();
	~StatePhase4();

	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
};

