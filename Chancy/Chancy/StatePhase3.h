#pragma once

#include "State.h"

class StatePhase3 : public State
{
public:
	StatePhase3();
	~StatePhase3();

	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
};

