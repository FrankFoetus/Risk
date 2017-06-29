#pragma once

#include <iostream>
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

class State
{
public:
	State();
	~State();

	void init(Bengine::InputManager* inputManager, Bengine::Camera2D* camera2D, Bengine::Camera2D* hudCamera, Bengine::AudioEngine* audioEngine, Level* level, 
		Bengine::GLSLProgram* colorProgram, Bengine::SpriteFont* spriteFont, Bengine::SpriteBatch* territoryBatch, Bengine::SpriteBatch* hudSpriteBatch, 
		Bengine::Window* window, const GLuint windowWidth, const GLuint windowHeight);

	virtual void enterState(Player* currentPlayer);
	virtual void processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState) = 0;

protected:
	void drawHud(std::string text, glm::vec2 pos, glm::vec2 size, Bengine::ColorRGBA8 color);
	virtual void leaveState(GameState& gameState);
	Territory* checkDistanceToTerritory(glm::vec2 mousePos);

	bool enteredState_ = false;
	float windowWidth_;
	float windowHeight_;
	Bengine::SpriteFont* spriteFont_;
	Bengine::InputManager* inputManager_;
	Bengine::AudioEngine* audioEngine_;
	Bengine::GLSLProgram* colorProgram_;
	Level* level_;
	Player* currentPlayer_;

	Bengine::Window* window_;
	Bengine::Camera2D* camera2D_;
	Bengine::Camera2D* hudCamera2D_;

	std::vector<Continent*> continents_; /// vector of continents
	std::vector<Territory*> territories_; /// vector of territories

	Bengine::SpriteBatch* hudSpritebatch_;
	Bengine::SpriteBatch* territoryBatch_;
};

