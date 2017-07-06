#pragma once

#include "State.h"

class StatePhase6 : public State
{
public:
	StatePhase6();
	~StatePhase6();

	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
	void enterState(int playerIndex);

private:
	virtual void leaveState(GameState& gameState);

	int movingUnits_ = 0;

	Territory* originTerritory_ = nullptr;
	Territory* destinationTerritory_ = nullptr;
};

