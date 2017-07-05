#include "StatePhase2.h"


StatePhase2::StatePhase2()
{
}


StatePhase2::~StatePhase2()
{
}


void StatePhase2::enterState(int playerIndex) {

	if (!enteredState_) {
		// set the current Player
		currentPlayer_ = players_[playerIndex];
		// get the number of reinforcements
		currentPlayer_->setEnergy(currentPlayer_->getEnergy() + calculateReinforcements());
		// dont enter state again before next round
		enteredState_ = true;
	}
}

void StatePhase2::processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState) {

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

	// process escape push
	if (inputManager_->isKeyPressed(SDLK_ESCAPE)) {
		// go into menu
		gameState = GameState::MENU;
		std::cout << "ENTERING MENU" << std::endl;
	}

	// process return click
	if (inputManager_->isKeyPressed(SDLK_RETURN)) {
		// go to the next phase
		gameState = GameState::PHASE3;
		std::cout << "ENTERING PHASE 3" << std::endl;
		leaveState(gameState);
	}

	// process left mouse click
	if (inputManager_->isKeyPressed(SDL_BUTTON_LEFT)) {
		// check if something was hit
		//checkDistanceToPurchases()
		// if space station was hit, player also needs to hit a territory
		if (spaceStationIsChosen_) {
			// add space station to cart

			// check if territory was hit by click
			Territory* territory = checkDistanceToTerritory(camera2D_->convertScreenToWorld(inputManager_->getMouseCoords()));
			if (territory != NULL) {
				// if territory is not lid up, light it up
				if (territory->getColor().a < 255) {
					// check if the maximum number of territories a space station can be placed is already reached
					if (numberOfTerritoriesChosen_ <= maxNumberOfSpaceStations) {
						territory->lightUpTerritory();
						numberOfTerritoriesChosen_++;
					}
					else {
						std::cout << "already too many territories chosen!" << std::endl;
					}
				}
				else {
					territory->lightDownTerritory();
					numberOfTerritoriesChosen_--;
				}
			}
		}
		// if something is hit, add it to cart
		//...
	}

}


void StatePhase2::drawGame() {

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
	drawHud("Phase 2: Buy commanders and space stations!", glm::vec2(-750, 360), glm::vec2(standardFontSize_), Bengine::ColorRGBA8(255, 255, 255, 255), true);
	drawHud(std::to_string(currentPlayer_->getEnergy()) + " energy left.", glm::vec2(-750, 330), glm::vec2(standardFontSize_), Bengine::ColorRGBA8(255, 255, 255, 255), true);

	// disable the shader
	colorProgram_->unuse();

	// setup our buffer and draw everything to the screen
	window_->swapBuffer();
}