
#include <algorithm>

#include <Bengine\BengineError.h>

#include "MainGame.h"


MainGame::MainGame()
{
	// empty constructor
}


MainGame::~MainGame()
{	
	for (auto level : levels_) {
		delete level;
	}
	levels_.clear();
	territories_.clear();
	for (auto player : players_) {
		delete player;
	}
	players_.clear();
	delete stateMenu_;
	delete statePhase1_;
	delete statePhase2_;
	delete statePhase3_;
	delete statePhase4_;
	delete statePhase5_;
	delete statePhase6_;
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
	players_.push_back(new Player("Mc LongSchlong", Bengine::ColorRGBA8(255, 0, 0, 255)));
	players_[0]->setTrunOrder(1);
	players_.push_back(new Player("Peter Pickel", Bengine::ColorRGBA8(0, 255, 0, 255)));
	players_[0]->setTrunOrder(2);
	players_.push_back(new Player("Shyla Schamhaar", Bengine::ColorRGBA8(0, 0, 255, 255)));
	players_[0]->setTrunOrder(3);
	// initialize level
	initLevel();
	// initialize 3 attacker dice and 2 defender dice
	attackerDice_ = new Dice(3, glm::vec2(windowWidth_ / 2 - 3 * dieSize_ - 30, windowHeight_ / 2 - 10), dieSize_);
	defenderDice_ = new Dice(2, glm::vec2(windowWidth_ / 2 - 3 * dieSize_ - 30, windowHeight_ / 2 - 20 - dieSize_), dieSize_);
	// initialize states
	stateMenu_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_, 
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, attackerDice_, defenderDice_, 
						windowWidth_, windowHeight_, players_, levels_[currentLevel_]->getContinents());
	statePhase1_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, attackerDice_, defenderDice_, 
						windowWidth_, windowHeight_, players_, levels_[currentLevel_]->getContinents());
	statePhase2_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, attackerDice_, defenderDice_, 
						windowWidth_, windowHeight_, players_, levels_[currentLevel_]->getContinents());
	statePhase3_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, attackerDice_, defenderDice_, 
						windowWidth_, windowHeight_, players_, levels_[currentLevel_]->getContinents());
	statePhase4_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, attackerDice_, defenderDice_, 
						windowWidth_, windowHeight_, players_, levels_[currentLevel_]->getContinents());
	statePhase5_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, attackerDice_, defenderDice_, 
						windowWidth_, windowHeight_, players_, levels_[currentLevel_]->getContinents());
	statePhase6_->init(&inputManager_, &camera2D_, &hudCamera2D_, &audioEngine_, levels_[currentLevel_], &colorProgram_,
						spriteFont_, &territoryBatch_, &hudSpritebatch_, &window_, attackerDice_, defenderDice_, 
						windowWidth_, windowHeight_, players_, levels_[currentLevel_]->getContinents());
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
	territories_ = levels_[currentLevel_]->getTerritories();

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
			stateMenu_->enterState(currentPlayer_);
			stateMenu_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			stateMenu_->drawGame();
			break;
		case GameState::PREPHASE:
			// choosing territories
			// TODO: set up the turn order by bidding energy
			// TODO: initial choosing of the territories
			break;
		case GameState::PHASE0:
			// changing player according to turn order
			if (currentPlayer_ < players_.size() - 1)
				currentPlayer_++;
			else
				currentPlayer_ = 0;
			gameState_ = GameState::PHASE1;
			std::cout << "Entering Phase 1" << std::endl;
			break;
		case GameState::PHASE1:
			// placing reinforcements
			inputManager_.update();
			statePhase1_->enterState(currentPlayer_);
			statePhase1_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase1_->drawGame();
			break;
		case GameState::PHASE2:
			// buying commanders and space stations
			inputManager_.update();
			statePhase2_->enterState(currentPlayer_);
			statePhase2_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase2_->drawGame();
			break;
		case GameState::PHASE3:
			// buying commander cards
			inputManager_.update();
			statePhase3_->enterState(currentPlayer_);
			statePhase3_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase3_->drawGame();
			break;
		case GameState::PHASE4:
			// playing commander cards
			inputManager_.update();
			statePhase4_->enterState(currentPlayer_);
			statePhase4_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase4_->drawGame();
			break;
		case GameState::PHASE5:
			// invade territories
			inputManager_.update();
			statePhase5_->enterState(currentPlayer_);
			statePhase5_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase5_->drawGame();
			break;
		case GameState::PHASE6:
			// move troops
			inputManager_.update();
			statePhase6_->enterState(currentPlayer_);
			statePhase6_->processInput(&cameraSpeed_, &scaleSpeed_, *&gameState_);
			statePhase6_->drawGame();
			break;
		}
		multiplayer_.sendData("Frame passed");

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