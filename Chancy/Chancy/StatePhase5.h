#pragma once

#include "State.h"


class StatePhase5 : public State
{
public:
	StatePhase5();
	~StatePhase5();

	virtual void enterState(int playerIndex);
	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();

private:
	virtual void leaveState(GameState& gameState);
	std::vector<Territory*> getNeighbours(Territory* territory);
	void processBattle();

	Territory* attackerTerritory_ = nullptr;
	Territory* defenderTerritory_ = nullptr;
	int attackingUnits_ = 0;
	int defendingUnits_ = 0;
};

