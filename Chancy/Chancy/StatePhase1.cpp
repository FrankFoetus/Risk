#include "StatePhase1.h"


StatePhase1::StatePhase1()
{
	//empty
}


StatePhase1::~StatePhase1()
{
	//empty
}


void StatePhase1::enterState(int playerIndex) {
	if (!enteredState_) {
		// set the current Player
		currentPlayer_ = players_[playerIndex];
		// light up all territories
		for (auto terr : territories_) {
			terr->lightUpTerritory(true);
		}
		// get the number of reinforcements
		numberOfReinforcements_ = calculateReinforcements();
		// dont enter state again before next round
		enteredState_ = true;
	}
}


void StatePhase1::leaveState(GameState& gameState) {
	// check if all troops have been placed
	if (numberOfReinforcements_ == 0) {
		// go to the next phase
		gameState = GameState::PHASE5;
		std::cout << "ENTERING PHASE 5" << std::endl;
		// update territories
		for (auto territory : territories_) {
			territory->updateUnitCount();
		}
		// make state enterable again
		enteredState_ = false;
	}
	else {
		std::cout << "Not all reinforcements have been placed!" << std::endl;
		// TODO: add sound to indicate remaining troops
	}
}


void StatePhase1::processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState) {

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
	}

	if (inputManager_->isKeyDown(SDLK_q)) {
		// zoom out
		if (camera2D_->getScale() > 0.5) {
			camera2D_->setScale(camera2D_->getScale() + -*scaleSpeed*camera2D_->getScale());
		}
	}
	if (inputManager_->isKeyDown(SDLK_e)) {
		// zoom in
		if (camera2D_->getScale() < 2) {
			camera2D_->setScale(camera2D_->getScale() + *scaleSpeed*camera2D_->getScale());
		}
	}
	if (inputManager_->isKeyDown(SDLK_r)) {
		// update camera position
		camera2D_->setPosition(glm::vec2(0.0, 0.0));
	}
	if (inputManager_->isKeyDown(SDLK_w)) {
		if (camera2D_->getPosition().y < windowHeight_ / 2) {  // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_->setPosition(camera2D_->getPosition() + glm::vec2(0.0, *cameraSpeed));
		}
	}
	if (inputManager_->isKeyDown(SDLK_s)) {
		if (camera2D_->getPosition().y > 0) { // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_->setPosition(camera2D_->getPosition() + glm::vec2(0.0, -*cameraSpeed));
		}
	}
	if (inputManager_->isKeyDown(SDLK_a)) {
		if (camera2D_->getPosition().x > 0) { // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_->setPosition(camera2D_->getPosition() + glm::vec2(-*cameraSpeed, 0.0));
		}
	}
	if (inputManager_->isKeyDown(SDLK_d)) {
		if (camera2D_->getPosition().x < windowWidth_ / 2) { // TODO: scale the borders for camera movement with camera zoom
			// update camera position
			camera2D_->setPosition(camera2D_->getPosition() + glm::vec2(*cameraSpeed, 0.0));
		}
	}

	// ESCAPE
	if (inputManager_->isKeyPressed(SDLK_ESCAPE)) {
		// go into menu
		gameState = GameState::MENU;
		std::cout << "ENTERING MENU" << std::endl;
	}

	// RETURN
	if (inputManager_->isKeyPressed(SDLK_RETURN)) {
		// leave the state
		leaveState(gameState);
	}

	// LEFT MOUSE CLICK
	if (inputManager_->isKeyPressed(SDL_BUTTON_LEFT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_->convertScreenToWorld(inputManager_->getMouseCoords()));
		if (territory != NULL) {
			// check if reinforcements are left
			if (numberOfReinforcements_ > 0) {
				if (territory->getOwner() == currentPlayer_ || territory->getOwner() == NULL) {
					// if selected territory is empty, the player gets it
					if (territory->getOwner() == NULL) {
						territory->setOwner(currentPlayer_);
					}
					territory->addUnit(audioEngine_);
					// decrement the number of troops that can be placed
					numberOfReinforcements_--;
				}
				else {
					std::cout << "This territory belongs to another player!" << std::endl;
				}
			}
			else {
				std::cout << "No more reinforcements left!" << std::endl;
			}
		}
	}

	// RIGHT MOUSE CLICK
	if (inputManager_->isKeyPressed(SDL_BUTTON_RIGHT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_->convertScreenToWorld(inputManager_->getMouseCoords()));
		if (territory != NULL) {
			// only if units have been placed in this phase, they can be removed
			if (territory->getUnitsT1().size() + territory->getUnitsT2().size() * 5 + territory->getUnitsT3().size() * 25 > territory->getUnitCount()) {
				territory->destroyUnit(audioEngine_);
				// increment number of troops that can be placed
				numberOfReinforcements_++;
			}
		}
	}
}

void StatePhase1::drawGame() {

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

	// show whos turn it is
	drawHud(currentPlayer_->getName() + "'s turn!", glm::vec2(-750, 390), glm::vec2(standardFontSize_), currentPlayer_->getColor(), true);
	// draw the hud
	drawHud("Phase 1: Place your reinforcements! ", glm::vec2(-750, 360), glm::vec2(standardFontSize_), Bengine::ColorRGBA8(255, 255, 255, 255), true);
	drawHud(std::to_string(numberOfReinforcements_) + " left.", glm::vec2(-750, 330), glm::vec2(standardFontSize_), Bengine::ColorRGBA8(255, 255, 255, 255), true);
	// disable the shader
	colorProgram_->unuse();

	// setup our buffer and draw everything to the screen
	window_->swapBuffer();
}
