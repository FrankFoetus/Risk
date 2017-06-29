#pragma once

#include "State.h"

class StatePhase1 : public State
{
public:
	StatePhase1();
	~StatePhase1();

	virtual void enterState(Player* currentPlayer);
	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();

private:
	virtual void leaveState(GameState& gameState);

	int calculateReinforcements();

	int numberOfReinforcements_ = 0;
};

