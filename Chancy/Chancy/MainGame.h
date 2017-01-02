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

#include <iostream>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include "Level.h"

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
	void checkVictory();

	void printFPS(int interval);

	std::vector<Level*> levels_; /// vector of levels
	int currentLevel_;

	Bengine::Window window_;
	Bengine::InputManager inputManager_;
	Bengine::GLSLProgram colorProgram_;
	Bengine::Camera2D camera2D_;
	Bengine::FpsLimiter fpsLimiter_;

	GLuint windowWidth_;
	GLuint windowHeight_;

	float cameraSpeed_;

	int framePrintCounter_;
	float fps_;
	float maxFPS_;

	GameState gameState_;
};

