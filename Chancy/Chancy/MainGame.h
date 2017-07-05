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
#include <Bengine\AudioEngine.h>

#include <iostream>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include "Level.h"
#include "Territory.h"
#include "Player.h"
#include "StateMenu.h"
#include "StatePhase1.h"
#include "StatePhase2.h"
#include "StatePhase3.h"
#include "StatePhase4.h"
#include "StatePhase5.h"
#include "StatePhase6.h"
#include "Util.h"
#include "Dice.h"

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

	void checkVictory();

	void printFPS(int interval);


	std::vector<Level*> levels_; /// vector of levels
	int currentLevel_;
	std::vector<Player*> players_; /// vector of players
	int currentPlayer_ = 0;
	std::vector<Territory*> territories_; /// vector of territories

	GLuint dieSize = 50;
	Dice* attackerDice_ = nullptr; /// vector of attacker dice
	Dice* defenderDice_ = nullptr; /// vector of defender dice

	Bengine::Window window_;
	Bengine::InputManager inputManager_;
	Bengine::GLSLProgram colorProgram_;
	Bengine::AudioEngine audioEngine_;
	Bengine::Camera2D camera2D_;
	Bengine::Camera2D hudCamera2D_;
	Bengine::SpriteFont* spriteFont_;

	Bengine::SpriteBatch hudSpritebatch_;
	Bengine::SpriteBatch territoryBatch_;

	Bengine::FpsLimiter fpsLimiter_;
	
	GLuint windowWidth_ = 1600;
	GLuint windowHeight_ = 900;

	float cameraSpeed_ = 5.f;
	float scaleSpeed_ = 0.025f;

	int framePrintCounter_ = 0;
	float fps_;
	float maxFPS_ = 60.f;

	GameState gameState_ = GameState::MENU;

	StateMenu* stateMenu_ = new StateMenu();
	StatePhase1* statePhase1_ = new StatePhase1();
	StatePhase2* statePhase2_ = new StatePhase2();
	StatePhase3* statePhase3_ = new StatePhase3();
	StatePhase4* statePhase4_ = new StatePhase4();
	StatePhase5* statePhase5_ = new StatePhase5();
	StatePhase6* statePhase6_ = new StatePhase6();
};

