#pragma once

#include <Bengine\Bengine.h>
#include <Bengine\SpriteBatch.h>
#include <Bengine\Window.h>
#include <Bengine\Camera2D.h>
#include <Bengine\InputManager.h>
#include <Bengine\GLTexture.h>
#include <Bengine\GLSLProgram.h>
#include <Bengine\Timing.h>
#include <Bengine\RessourceManager.h>
#include <Bengine\SpriteFont.h>

#include <iostream>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include "Level.h"
#include "Territory.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void gameLoop();
	void initSys();
	void initLevel();
	void initShaders();

	void processInput();
	void drawGame();
	void drawHud();
	void checkVictory();
	Territory* checkDistanceToTerritory(glm::vec2 mousePos);

	void printFPS(int interval);

	std::vector<Level*> levels_; /// vector of levels
	std::vector<Territory*> territories_; /// vector of territories
	int currentLevel_;

	Bengine::Window window_;
	Bengine::InputManager inputManager_;
	Bengine::GLSLProgram colorProgram_;

	Bengine::Camera2D camera2D_;
	Bengine::Camera2D hudCamera2D_;

	Bengine::FpsLimiter fpsLimiter_;

	Bengine::SpriteFont* spriteFont_;

	Bengine::SpriteBatch hudSpritebatch_;
	Bengine::SpriteBatch territoryBatch_;
	Bengine::SpriteBatch unitsT1Batch_;
	Bengine::SpriteBatch unitsT2Batch_;

	GLuint windowWidth_;
	GLuint windowHeight_;

	float cameraSpeed_;

	int framePrintCounter_;
	float fps_;
	float maxFPS_;

	GameState gameState_;
};

