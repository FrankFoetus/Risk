
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
	initLevel();

	Bengine::Music music = audioEngine_.loadMusic("Sounds/Siste Viator.ogg");
	music.play(-1);

	gameLoop();
}


void MainGame::initSys() {
	Bengine::init();

	// initlialize sound, must happen after Bengine init
	audioEngine_.init();

	window_.create("Chancy", windowWidth_, windowHeight_, 0);

	initShaders();

	fpsLimiter_.init(maxFPS_);

	camera2D_.init(windowWidth_, windowHeight_);
	hudCamera2D_.init(windowWidth_, windowHeight_);
	hudCamera2D_.setPosition(glm::vec2(windowWidth_ / 2 ,windowHeight_ / 2));

	hudSpritebatch_.init();
	territoryBatch_.init();

	// initialize the spriteFont
	spriteFont_ = new Bengine::SpriteFont("Fonts/chintzy.ttf", 32);
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

	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMTIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float previousTicks = SDL_GetTicks();

	float newTicks = SDL_GetTicks();
	float frameTime = newTicks - previousTicks;
	previousTicks = newTicks;
	float totalDeltaTime = frameTime / DESIRED_FRAMTIME;


	// main loop
	while (gameState_ != GameState::EXIT) {
		fpsLimiter_.begin();
		// do actions according to gameState_
		switch (gameState_)
		{
		case GameState::MENU:

			processInput();

			break;
		case GameState::PLAY:

			checkVictory();

			inputManager_.update();

			processInput();

			int i = 0;
			while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
				float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

				// TODO: updateTerritories()

				totalDeltaTime -= deltaTime;
				i++;
			}

			break;
		}

		camera2D_.update();
		hudCamera2D_.update();

		drawGame();

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


void MainGame::processInput() {
	const float SCALE_SPEED = 0.025f;

	SDL_Event evt;

	while (SDL_PollEvent(&evt)) {

		switch (evt.type)
		{
		case SDL_QUIT:
			gameState_ = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			inputManager_.pressKey(evt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager_.releaseKey(evt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager_.pressKey(evt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager_.releaseKey(evt.button.button);
			break;
		case SDL_MOUSEMOTION:
			inputManager_.setMouseCoords(evt.motion.x, evt.motion.y);
			//std::cout << evt.motion.x << " " << evt.motion.y << std::endl;
			break;
		default:
			break;
		}
	}

	if (inputManager_.isKeyDown(SDLK_q)) {
		if (camera2D_.getScale() > 0.5) {
			camera2D_.setScale(camera2D_.getScale() + -SCALE_SPEED*camera2D_.getScale());
		}
	}
	if (inputManager_.isKeyDown(SDLK_e)) {
		if (camera2D_.getScale() < 2) {
			camera2D_.setScale(camera2D_.getScale() + SCALE_SPEED*camera2D_.getScale());
		}
	}
	if (inputManager_.isKeyDown(SDLK_r)) {
		// update camera position
		camera2D_.setPosition(glm::vec2(0.0, 0.0));
	}
	if (inputManager_.isKeyDown(SDLK_w)) {
		if (camera2D_.getPosition().y < windowHeight_/2) {  // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_.setPosition(camera2D_.getPosition() + glm::vec2(0.0, cameraSpeed_ /* * deltaTime */));
		}
	}
	if (inputManager_.isKeyDown(SDLK_s)) {
		if (camera2D_.getPosition().y > 0) { // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_.setPosition(camera2D_.getPosition() + glm::vec2(0.0, -cameraSpeed_ /* * deltaTime */));
		}
	}
	if (inputManager_.isKeyDown(SDLK_a)) {
		if (camera2D_.getPosition().x > 0) { // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_.setPosition(camera2D_.getPosition() + glm::vec2(-cameraSpeed_ /* * deltaTime */, 0.0));
		}
	}
	if (inputManager_.isKeyDown(SDLK_d)) {
		if (camera2D_.getPosition().x < windowWidth_ / 2) { // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_.setPosition(camera2D_.getPosition() + glm::vec2(cameraSpeed_ /* * deltaTime*/, 0.0));
		}
	}

	// process left mouse click
	if (inputManager_.isKeyPressed(SDL_BUTTON_LEFT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_.convertScreenToWorld(inputManager_.getMouseCoords()));
		if (territory != NULL) {
			// turn territory black for testing purposes
			if (territory->getColor().a < 255) {
				lightUpTerritory(territory);
			}
			else {
				lightDownTerritory(territory);
			}
		}
	}
	// process right mouse click
	if (inputManager_.isKeyPressed(SDL_BUTTON_RIGHT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_.convertScreenToWorld(inputManager_.getMouseCoords()));
		if (territory != NULL) {
			// if left control is pressed, remove units
			if (inputManager_.isKeyDown(SDLK_LCTRL)) {
				// if there are no more T1 units, delete one T2 unit (if available) and spawn four T1 units
				if (territory->getUnitsT1().size() == 0) {
					if (territory->getUnitsT2().size() == 0) {
						// if there are no more T2 units, delete one T3 unit (if available) and spawn four T1 units
						if (territory->getUnitsT3().size() != 0) {
							unitTransformation(territory, 3, 2);
						}
						else {
							std::cout << "NO MORE UNITS HERE!!" << std::endl;
						}
					} else {
						unitTransformation(territory, 2, 1);
					}
				} else {
					unitTransformation(territory, 1, 0);
				}
			// if left control is not pressed, add units
			} else {
				// if there are already four T1 units, delete them and spawn one T2 unit
				if (territory->getUnitsT1().size() == 4) {
					// if there are already four T2 units, delete them and spawn one T3 unit
					if (territory->getUnitsT2().size() == 4) {
						unitTransformation(territory, 2, 3);
						territory->getUnitsT3()[territory->getUnitsT3().size() - 1]->getSpawnSoundEffect().play();
					}
					else {
						unitTransformation(territory, 1, 2);
						territory->getUnitsT2()[territory->getUnitsT2().size() - 1]->getSpawnSoundEffect().play();
					}
				}
				else {
					// spawn one T1
					unitTransformation(territory, 0, 1);
					territory->getUnitsT1()[territory->getUnitsT1().size() - 1]->getSpawnSoundEffect().play();
				}
				lightUpTerritory(territory);
			}
		}
	}
}


void MainGame::drawGame() {

	// Set the depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer (  | <-- this is binary or)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// enable the shader
	colorProgram_.use();

	// we are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	// get the uniform location
	GLint textureLocation = colorProgram_.getUniformLocation("mySampler");
	// tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	/*
	* // set the currently changing time variable
	* GLint timeLocation = colorProgram_.getUniformLocation("time");
	* glUniform1f(timeLocation, _time);
	*/

	// set the camera matrix
	GLint pLocation = colorProgram_.getUniformLocation("P");
	glm::mat4 cameraMatrix = camera2D_.getCameraMatrix();

	// upload matrix to the gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	// draw the territories
	territoryBatch_.begin();

	for (auto it : territories_) {
		// draw all territories to the sprite batch
		it->draw(&territoryBatch_);
	}

	territoryBatch_.end();
	// render the territories
	territoryBatch_.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// draw the hud
	drawHud();

	// disable the shader
	colorProgram_.unuse();

	// setup our buffer and draw everything to the screen
	window_.swapBuffer();
}


void MainGame::drawHud() {
	// set the camera matrix
	glm::mat4 cameraMatrix = hudCamera2D_.getCameraMatrix();
	GLint pLocation = colorProgram_.getUniformLocation("P");
	// upload matrix to the gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	char buffer[256];

	hudSpritebatch_.begin();

	sprintf_s(buffer, "Welcome to Chancy!");
	spriteFont_->draw(hudSpritebatch_, buffer, glm::vec2(500,800), 
						glm::vec2(2.0), 0.0f, Bengine::ColorRGBA8(255,255,255,255));

	hudSpritebatch_.end();
	hudSpritebatch_.renderBatch();
}


/**
	checks if there are any territories hit by mouse click
*/
Territory* MainGame::checkDistanceToTerritory(glm::vec2 mousePos) {
	for (auto it : levels_[currentLevel_]->getTerritories()) {
		glm::vec4 territoryPos = it->getPosition();
		/*
		std::cout << territoryPos.x << " " << territoryPos.y << std::endl;
		std::cout << mousePos.x << " " << mousePos.y << std::endl;
		*/
		if (mousePos.x < territoryPos.x + territoryPos.z && mousePos.x > territoryPos.x && mousePos.y > territoryPos.y && mousePos.y < territoryPos.y + territoryPos.w) {
			return it;
		}
	}
	return nullptr;
}


/**
performs a unit transformation from tier Tx into tier Ty in territory
*/
void MainGame::unitTransformation(Territory* territory, int Tx, int Ty) {
	std::vector<UnitT1*> unitsT1 = territory->getUnitsT1();
	std::vector<UnitT2*> unitsT2 = territory->getUnitsT2();
	std::vector<UnitT3*> unitsT3 = territory->getUnitsT3();
	switch (Tx) {
	case 1:
		if (Tx < Ty) {
			// delete four T1 units (if they are to be transformed into one T2 unit)
			for (unsigned int i = 0; i < 4; i++) {
				delete unitsT1[i];
			}
			unitsT1.erase(unitsT1.begin(), unitsT1.end());
		} else {
			// delete only one T1 unit (if it is to be removed)
			delete unitsT1[unitsT1.size() - 1];
			unitsT1.pop_back();
		}
		break;
	case 2:
		if (Tx < Ty) {
			// delete four T2 and four T1 units (if they are to be transformed into one T3 unit)
			for (unsigned int i = 0; i < 4; i++) {
				delete unitsT2[3 - i];
				delete unitsT1[3 - i];
				unitsT2.pop_back();
				unitsT1.pop_back();
			}
		} else {
			// delete only one T2 unit (if it is to be transformed into four T1 units)
			delete unitsT2[unitsT2.size() - 1];
			unitsT2.pop_back();
		}
		break;
	case 3:
		// delete one T3 units (can only go from one T3 unit to four T2 and four T1 units)
		delete unitsT3[unitsT3.size() - 1];
		unitsT3.pop_back();
		break;
	default:
		break;
	}
	switch (Ty) {
	case 1:
		unsigned int j;
		if (Tx < Ty) { j = 1; }  /// if only spawn one T1 unit
		else		 { j = 4; }  /// if transform a T2 unit into four T1

		// add j new T1 unit to the unitsT1 vector
		for (unsigned int i = 0; i < j; i++) {
			unitsT1.emplace_back(new UnitT1(glm::vec4(territory->getPosition().x + (territory->getUnitsT1().size() + i) * territory->getPosition().z / 4,
				territory->getPosition().y, territory->getPosition().z / 4, territory->getPosition().w / 3),
				territory->getUV(),
				Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Mushroom1.png").id,
				Bengine::ColorRGBA8(255, 255, 255, 150),
				audioEngine_.loadSoundEffect("Sounds/slimy_monster1.mp3")));
		}
		break;
	case 2:
		// if four T1 are to be transformed into one T2
		if (Tx < Ty) {
			// add one new T2 unit to the unitsT2 vector
			unitsT2.emplace_back(new UnitT2(glm::vec4(territory->getPosition().x + territory->getUnitsT2().size() * territory->getPosition().z / 4,
				territory->getPosition().y + territory->getPosition().w / 3, territory->getPosition().z / 4, territory->getPosition().w / 3),
				territory->getUV(),
				Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Broccoli1.png").id,
				Bengine::ColorRGBA8(255, 255, 255, 150),
				audioEngine_.loadSoundEffect("Sounds/slimy_monster2.mp3")));
		}
		// if one T3 unit is to be transformed into four T2 and four T1 units
		else { 
			for (unsigned int i = 0; i < 4; i++) {
				// add a new T2 unit to the unitsT2 vector
				unitsT2.emplace_back(new UnitT2(glm::vec4(territory->getPosition().x + (territory->getUnitsT2().size() + i) * territory->getPosition().z / 4,
					territory->getPosition().y + territory->getPosition().w / 3, territory->getPosition().z / 4, territory->getPosition().w / 3),
					territory->getUV(),
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Broccoli1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 150),
					audioEngine_.loadSoundEffect("Sounds/slimy_monster2.mp3")));
				// add a new T3 unit to the unitsT3 vector
				unitsT1.emplace_back(new UnitT1(glm::vec4(territory->getPosition().x + (territory->getUnitsT1().size() + i) * territory->getPosition().z / 4,
					territory->getPosition().y, territory->getPosition().z / 4, territory->getPosition().w / 3),
					territory->getUV(),
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Mushroom1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 150),
					audioEngine_.loadSoundEffect("Sounds/slimy_monster1.mp3")));
			}
		}  

		break;
	case 3:
		// add one new T3 unit to the unitsT3 vector
		unitsT3.emplace_back(new UnitT3(glm::vec4(territory->getPosition().x + territory->getUnitsT3().size() * territory->getPosition().z / 4,
			territory->getPosition().y + territory->getPosition().w * 2 / 3, territory->getPosition().z / 4, territory->getPosition().w / 3),
			territory->getUV(),
			Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Candy1.png").id,
			Bengine::ColorRGBA8(255, 255, 255, 150),
			audioEngine_.loadSoundEffect("Sounds/slimy_monster.mp3")));
		break;
	default:
		break;
	}
	territory->setUnitsT1(unitsT1);
	territory->setUnitsT2(unitsT2);
	territory->setUnitsT3(unitsT3);
}


/**
	light up all units and the sprite of territory
*/
void MainGame::lightUpTerritory(Territory* territory) {
	territory->setColor(Bengine::ColorRGBA8(territory->getColor().r, territory->getColor().g, territory->getColor().b, 255));
	for (auto it : territory->getUnitsT1()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
	for (auto it : territory->getUnitsT2()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
	for (auto it : territory->getUnitsT3()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
}


/**
	light down all units and the sprite of territory
*/
void MainGame::lightDownTerritory(Territory* territory) {
	territory->setColor(Bengine::ColorRGBA8(territory->getColor().r, territory->getColor().g, territory->getColor().b, 150));
	for (auto it : territory->getUnitsT1()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
	for (auto it : territory->getUnitsT2()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
	for (auto it : territory->getUnitsT3()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
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