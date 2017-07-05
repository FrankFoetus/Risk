#pragma once

#include <iostream>
#include <time.h>
#include <algorithm>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include <Bengine\SpriteBatch.h>
#include <Bengine\Camera2D.h>
#include <Bengine\InputManager.h>
#include <Bengine\AudioEngine.h>
#include <Bengine\GLSLProgram.h>
#include <Bengine\SpriteFont.h>
#include <Bengine\Window.h>

#include "Level.h"
#include "Util.h"
#include "Territory.h"
#include "Dice.h"

class State
{
public:
	State();
	~State();

	void init(Bengine::InputManager* inputManager, Bengine::Camera2D* camera2D, Bengine::Camera2D* hudCamera, Bengine::AudioEngine* audioEngine, Level* level, 
		Bengine::GLSLProgram* colorProgram, Bengine::SpriteFont* spriteFont, Bengine::SpriteBatch* territoryBatch, Bengine::SpriteBatch* hudSpriteBatch, 
		Bengine::Window* window, Dice* attackerDice, Dice* defenderDice, const GLuint windowWidth, const GLuint windowHeight, std::vector<Player*> players);

	virtual void enterState(int currentPlayer);
	virtual void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState) = 0;

protected:
	void drawHud(std::string text, glm::vec2 pos, glm::vec2 size, Bengine::ColorRGBA8 color, bool stationary);
	
	virtual void leaveState(GameState& gameState);
	Territory* checkDistanceToTerritory(glm::vec2 mousePos);

	bool leaveOK_ = true;
	bool enteredState_ = false;
	bool diceActivatited_ = false;
	float windowWidth_ = 0;
	float windowHeight_ = 0;
	int waitTime_;
	Bengine::SpriteFont* spriteFont_ = nullptr;
	Bengine::InputManager* inputManager_ = nullptr;
	Bengine::AudioEngine* audioEngine_ = nullptr;
	Bengine::GLSLProgram* colorProgram_ = nullptr;
	Level* level_ = nullptr;
	Player* currentPlayer_ = nullptr;

	Bengine::Window* window_ = nullptr;
	Bengine::Camera2D* camera2D_ = nullptr;
	Bengine::Camera2D* hudCamera2D_ = nullptr;

	std::vector<Continent*> continents_; /// vector of continents
	std::vector<Territory*> territories_; /// vector of territories
	std::vector<Player*> players_; /// vector of players
	Dice* attackerDice_; /// vector of attacker dice
	Dice* defenderDice_; /// vector of defender dice

	Bengine::SpriteBatch* hudSpritebatch_ = nullptr;
	Bengine::SpriteBatch* territoryBatch_ = nullptr;
};

