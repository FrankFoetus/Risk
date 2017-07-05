#pragma once

#include "State.h"

class StateMenu : public State
{
public:
	StateMenu();
	~StateMenu();

	void enterState(int playerIndex);
	void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState);
	void drawGame();
private:
};

