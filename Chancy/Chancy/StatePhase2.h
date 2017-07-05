#pragma once

#include "State.h"

class StatePhase2 : public State
{
public:
	StatePhase2();
	~StatePhase2();

	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
	void enterState(int playerIndex);

private:
	// needed for the purchase logic
	const int maxNumberOfSpaceStations = 5;
	int numberOfTerritoriesChosen_ = 0;
	bool spaceStationIsChosen_ = false;
};

