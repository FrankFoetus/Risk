#pragma once

#include "State.h"

class StatePhase5 : public State
{
public:
	StatePhase5();
	~StatePhase5();

	virtual void enterState(int currentPlayer);
	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();

private:
	virtual void leaveState(GameState& gameState);
	std::vector<Territory*> getNeighbours(Territory* territory);

	Territory* attackerTerritory_;
	Territory* defenderTerritory_;
};

