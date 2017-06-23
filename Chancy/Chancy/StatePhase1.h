#pragma once

#include "State.h"

class StatePhase1 : public State
{
public:
	StatePhase1();
	~StatePhase1();

	void enterState(Player* currentPlayer);
	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();

private:
	int calculateReinforcements();

	int numberOfReinforcements_ = 0;
};

