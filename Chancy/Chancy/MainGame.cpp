
#include <algorithm>

#include <Bengine\BengineError.h>

#include "MainGame.h"


MainGame::MainGame()
{
	// empty constructor
}


MainGame::~MainGame()
{
	for (GLuint i = 0; i < levels_.size(); i++) {
		delete levels_[i];
	}
}


void MainGame::run() {
	initSys();

	Bengine::Music music = audioEngine_.loadMusic("Sounds/Siste Viator.ogg");
	//music.play(-1);

	gameLoop();
}


void MainGame::initSys() {
	Bengine::init();

	// initlialize sound, must happen after Bengine init
	audioEngine_.init();

	window_.create("Chancy", windowWidth_, windowHeight_, 0);

	initShaders();

	fpsLimiter_.init(maxFPS_);

	// initialize cameras
	camera2D_.init(windowWidth_, windowHeight_);
	hudCamera2D_.init(windowWidth_, windowHeight_);
	// initialize sprite batches
	territoryBatch_.init();
	hudSpritebatch_.init();
	// initialize the spriteFont
	spriteFont_ = new Bengine::SpriteFont("Fonts/chintzy.ttf", 32);

	// initialize Players
	players_.push_back(new Player());
	// initialize level
	initLevel();
	// initialize states
	stateMenu_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_, 
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, windowWidth_, windowHeight_);
	statePhase1_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, windowWidth_, windowHeight_);
	statePhase2_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, windowWidth_, windowHeight_);
	statePhase3_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, windowWidth_, windowHeight_);
	statePhase4_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, windowWidth_, windowHeight_);
	statePhase5_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, windowWidth_, windowHeight_);
	statePhase6_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, windowWidth_, windowHeight_);
}


void MainGame::initShaders() {
	colorProgram_.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	colorProgram_.addAttribute("vertexPosition");
	colorProgram_.addAttribute("vertexColor");
	colorProgram_.addAttribute("vertexUV");
	colorProgram_.linkShader();
}


void MainGame::initLevel() {

	levels_.push_back(new Level("Levels/Level1.txt", windowHeight_, windowWidth_));
	currentLevel_ = 0;
	territories_ = levels_[0]->getTerritories();
}


void MainGame::gameLoop() {

	// defines the fps that are supposed to be
	const float DESIRED_FPS = 60.0f;

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMTIME = MS_PER_SECOND / DESIRED_FPS;
	
	// main loop
	while (gameState_ != GameState::EXIT) {
		fpsLimiter_.begin();
		// do actions according to gameState_
		switch (gameState_)
		{
		case GameState::MENU:

			inputManager_.update();
			stateMenu_->enterState(players_[0]);
			stateMenu_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			stateMenu_->drawGame();
			break;
		case GameState::PHASE1:

			inputManager_.update();
			statePhase1_->enterState(players_[0]);
			statePhase1_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase1_->drawGame();
			break;
		case GameState::PHASE2:

			inputManager_.update();
			statePhase2_->enterState(players_[0]);
			statePhase2_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase2_->drawGame();
			break;
		case GameState::PHASE3:

			inputManager_.update();
			statePhase3_->enterState(players_[0]);
			statePhase3_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase3_->drawGame();
			break;
		case GameState::PHASE4:

			inputManager_.update();
			statePhase4_->enterState(players_[0]);
			statePhase4_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase4_->drawGame();
			break;
		case GameState::PHASE5:

			inputManager_.update();
			statePhase5_->enterState(players_[0]);
			statePhase5_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase5_->drawGame();
			break;
		case GameState::PHASE6:

			inputManager_.update();
			statePhase6_->enterState(players_[0]);
			statePhase6_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase6_->drawGame();
			break;
		}

		fps_ = fpsLimiter_.end();
		//printFPS(100);
	}
}


void MainGame::checkVictory() {
	// TODO: Support for multiple levels!
	// if x, player a wins
	/*
	* for (int indPlayer = 0; indPlayer < players_.size(); indPlayer++){
	*	if (players_[indPlayer].hasWon) {
	*		std::printf("*** !!!PLAYER %d WINS!!! ***\n", players_[indPLayer].getName());
	*	}
	*}
	*/
}

/**
	print current fps to the console
*/
void MainGame::printFPS(int interval) {
	if (framePrintCounter_ == interval) {
		std::cout << fps_ << std::endl;
		framePrintCounter_ = 1;
	}
	else
	{
		framePrintCounter_++;
	}
}