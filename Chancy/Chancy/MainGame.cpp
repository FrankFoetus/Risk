
#include <algorithm>

#include <Bengine\Error.h>

#include "MainGame.h"


MainGame::MainGame() :
	gameState_(GameState::PLAY),
	windowWidth_(1600),
	windowHeight_(900),
	maxFPS_(60),
	framePrintCounter_(0),
	cameraSpeed_(5)
{
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

	gameLoop();
}


void MainGame::initSys() {
	Bengine::init();

	window_.create("Chancy", windowWidth_, windowHeight_, 0);

	initShaders();

	fpsLimiter_.init(maxFPS_);

	camera2D_.init(windowWidth_, windowHeight_);
	hudCamera2D_.init(windowWidth_, windowHeight_);
	hudCamera2D_.setPosition(glm::vec2(windowWidth_ / 2 ,windowHeight_ / 2));

	hudSpritebatch_.init();
	territoryBatch_.init();
	unitsT1Batch_.init();
	unitsT2Batch_.init();

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

	// main loop
	while (gameState_ != GameState::EXIT) {

		fpsLimiter_.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMTIME;

		switch (gameState_) 
		{
		case GameState::PLAY:
			checkVictory();
			break;
		default:
			break;
		}

		inputManager_.update();

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			// TODO: updateTerritories()

			totalDeltaTime -= deltaTime;
			i++;
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
				territory->setColor(Bengine::ColorRGBA8(territory->getColor().r, territory->getColor().g, territory->getColor().b, 255));
				for (auto it : territory->getUnitsT1()) {
					it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
				}
			}
			else {
				territory->setColor(Bengine::ColorRGBA8(territory->getColor().r, territory->getColor().g, territory->getColor().b, 150));
				for (auto it : territory->getUnitsT1()) {
					it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
				}
			}
		}
	}
	// process right mouse click
	if (inputManager_.isKeyPressed(SDL_BUTTON_RIGHT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_.convertScreenToWorld(inputManager_.getMouseCoords()));
		if (territory != NULL) {
			std::vector<Unit*> unitsT1 = territory->getUnitsT1();
			std::vector<Unit*> unitsT2 = territory->getUnitsT2();
			if (unitsT1.size() == 5) {
				unitsT2.emplace_back(new Unit(glm::vec4(territory->getPosition().x + territory->getUnitsT2().size() * territory->getPosition().z / 5,
						territory->getPosition().y + territory->getPosition().w / 3, territory->getPosition().z / 5, territory->getPosition().w / 3),
					territory->getUV(),
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Broccoli1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 150)));
				// delete 5 T1 units 
				for (unsigned int i = 0; i < 5; i++) {
					delete unitsT1[i];
				}
				unitsT1.erase(unitsT1.begin(), unitsT1.end());
			}
			else {
				unitsT1.emplace_back(new Unit(glm::vec4(territory->getPosition().x + territory->getUnitsT1().size() * territory->getPosition().z / 5,
						territory->getPosition().y, territory->getPosition().z / 5, territory->getPosition().w / 3),
					territory->getUV(),
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Mushroom1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 150)));
			}
			territory->setUnitsT1(unitsT1);
			territory->setUnitsT2(unitsT2);
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
	// sprite batch for units
	unitsT1Batch_.begin();
	unitsT2Batch_.begin();

	for (auto it : territories_) {
		// draw all territories to the sprite batch
		it->draw(&territoryBatch_, &unitsT1Batch_, &unitsT2Batch_);
	}

	territoryBatch_.end();
	unitsT1Batch_.end();
	unitsT2Batch_.end();
	// render the territories
	territoryBatch_.renderBatch();
	unitsT1Batch_.renderBatch();
	unitsT2Batch_.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// draw the hud
	//drawHud();

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

	sprintf_s(buffer, "abcdefgh");
	spriteFont_->draw(hudSpritebatch_, buffer, glm::vec2(100,100), 
						glm::vec2(4.0), 0.0f, Bengine::ColorRGBA8(255,255,255,255));

	hudSpritebatch_.end();
	hudSpritebatch_.renderBatch();
}


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
