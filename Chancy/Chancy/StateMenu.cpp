#include "StateMenu.h"



StateMenu::StateMenu()
{
}


StateMenu::~StateMenu()
{
}


void StateMenu::enterState(int currentPlayer) {
	// set the current Player
	currentPlayer_ = players_[currentPlayer];
	// light down all territories
	for (auto terr : territories_) {
		terr->lightDownTerritory();
	}
}


void StateMenu::processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState) {

	SDL_Event evt;

	while (SDL_PollEvent(&evt)) {
		switch (evt.type)
		{
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			inputManager_->pressKey(evt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager_->releaseKey(evt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager_->pressKey(evt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager_->releaseKey(evt.button.button);
			break;
		case SDL_MOUSEMOTION:
			inputManager_->setMouseCoords(evt.motion.x, evt.motion.y);
			break;
		default:
			break;
		}

		// RETURN
		if (inputManager_->isKeyPressed(SDLK_RETURN)) {
			// go to the next phase
			gameState = GameState::PHASE1;
			std::cout << "ENTERING PHASE 1" << std::endl;
			leaveState(gameState);
		}

		// ESCAPE
		if (inputManager_->isKeyPressed(SDLK_ESCAPE)) {
			// exit the game
			gameState = GameState::EXIT;
			std::cout << "EXETING GAME" << std::endl;
		}
	}
}


void StateMenu::drawGame() {

	camera2D_->update();
	hudCamera2D_->update();

	// Set the depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer (  | <-- this is binary or)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// enable the shader
	colorProgram_->use();

	// we are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	// get the uniform location
	GLint textureLocation = colorProgram_->getUniformLocation("mySampler");
	// tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	/*
	* // set the currently changing time variable
	* GLint timeLocation = colorProgram_.getUniformLocation("time");
	* glUniform1f(timeLocation, _time);
	*/

	// set the camera matrix
	GLint pLocation = colorProgram_->getUniformLocation("P");
	glm::mat4 cameraMatrix = camera2D_->getCameraMatrix();

	// upload matrix to the gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	// draw the territories
	territoryBatch_->begin();
	for (auto it : territories_) {
		// draw (add) all territories to the sprite batch
		it->draw(territoryBatch_);
	}
	territoryBatch_->end();
	// render the territories
	territoryBatch_->renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// draw the hud
	drawHud("This is the menu", glm::vec2(-700, 360), glm::vec2(2), Bengine::ColorRGBA8(255, 255, 255, 255), true);
	drawHud("Press return to start a new round!", glm::vec2(-550, 0), glm::vec2(2), Bengine::ColorRGBA8(255, 255, 255, 255), true);
	drawHud("Press escape to exit the game!", glm::vec2(-480, -100), glm::vec2(2), Bengine::ColorRGBA8(255, 255, 255, 255), true);

	// disable the shader
	colorProgram_->unuse();

	// setup our buffer and draw everything to the screen
	window_->swapBuffer();
}
