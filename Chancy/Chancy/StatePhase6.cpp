#include "StatePhase6.h"



StatePhase6::StatePhase6()
{
}


StatePhase6::~StatePhase6()
{
}


void StatePhase6::enterState(int playerIndex) {
	if (!enteredState_) {
		std::cout << "ENTERING STATE 5" << std::endl;
		// set the current Player
		currentPlayer_ = players_[playerIndex];
		// light down all territories
		for (auto terr : territories_) {
			terr->lightDownTerritory(true);
		}
		// dont enter state again before next round
		enteredState_ = true;
	}
}


void StatePhase6::leaveState(GameState& gameState) {
	// go to the next phase
	gameState = GameState::PHASE0;
	std::cout << "ENTERING PHASE 0" << std::endl;
	// make state enterable again
	enteredState_ = false;
	// reset all moved units
	for (auto territory : territories_) {
		territory->setMovedUnitsCount(0);
	}
}


void StatePhase6::processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState) {

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
		if (movingUnits_ > 0) {
			// process invasion
			diceActivatited_ = true;
		}
		else {
			if (leaveOK_) {
				// leave the state
				leaveState(gameState);
			}
		}
	}

	// LEFT MOUSE CLICK
	if (inputManager_->isKeyPressed(SDL_BUTTON_LEFT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_->convertScreenToWorld(inputManager_->getMouseCoords()));
		if (territory != NULL) {
			// if territory was not lid up, light it and its neighbours up and all other territories down 
			if (territory->getColor().a < 255) {
				if (territory->getOwner() == currentPlayer_) {
					// make state not exitable
					leaveOK_ = false;
					// CHANGING ORIGIN:
					// light down all territories
					for (auto terr : territories_) {
						terr->lightDownTerritory(true);
					}
					if (originTerritory_ != nullptr) {
						// reset color of origin territory
						originTerritory_->resetColor();
					}
					// mark the selected territory as the origin
					originTerritory_ = territory;
					movingUnits_ = 0;
					// adjust color and size of origin territory
					originTerritory_->setColor(Bengine::ColorRGBA8(255, 255, 255, 255));
					// reset the destination
					if (destinationTerritory_ != nullptr) {
						destinationTerritory_->resetColor();
						destinationTerritory_ = nullptr;
					}

					// light up origin territory and its neighbours
					originTerritory_->lightUpTerritory(true);
					for (auto terr : getNeighbours(originTerritory_)) {
						// only light up territories of the current player that can be reached
						if (terr->getOwner() == currentPlayer_) {
							terr->lightUpTerritory(true);
						}
					}
				}
				else {
					std::cout << "This territory does not belong to You!" << std::endl;
				}
			}
			// if territory is already lid up
			else {
				if (territory == originTerritory_) {
					// deselect the territory and its neighbours
					for (auto territory : territories_) {
						territory->lightDownTerritory(true);
					}
					// reset color of origin and destination territories
					originTerritory_->resetColor();
					if (destinationTerritory_ != nullptr) {
						destinationTerritory_->resetColor();
					}
					// unmark the origin and destination territory
					originTerritory_ = nullptr;
					destinationTerritory_ = nullptr;
					// make ready for going to next state
					leaveOK_ = true;
				}
				// if selected territory is lid up and not the origin territory
				else {
					if (destinationTerritory_ != nullptr) {
						// reset the color of the destination territory
						destinationTerritory_->resetColor();
						destinationTerritory_->lightUpTerritory(true);
					}
					// choose number of units to move
					destinationTerritory_ = territory;
					movingUnits_ = 0;
					// adjust color of destination territories
					destinationTerritory_->setColor(Bengine::ColorRGBA8(255, 255, 255, 255));
				}
			}
		}
	}

	// UP
	if (inputManager_->isKeyPressed(SDLK_UP)) {
		// if attacker and defender are chosen, count up troops to attack
		if (originTerritory_ != nullptr && destinationTerritory_ != nullptr) {
			// if shift is pushed add 5 or maximum units in territory
			if (inputManager_->isKeyDown(SDLK_LSHIFT)) {
				if (originTerritory_->getMovedUnitsCount() > 0) {
					movingUnits_ = std::min(movingUnits_ + 5, originTerritory_->getUnitCount() - originTerritory_->getMovedUnitsCount());
				}
				else {
					movingUnits_ = std::min(movingUnits_ + 5, originTerritory_->getUnitCount() - 1 - originTerritory_->getMovedUnitsCount());
				}
			}
			// if attacking units are less than units in attacking territory, increment
			if (originTerritory_->getMovedUnitsCount() > 0) {
				movingUnits_ = std::min(movingUnits_ + 1, originTerritory_->getUnitCount() - originTerritory_->getMovedUnitsCount());
			}
			else {
				movingUnits_ = std::min(movingUnits_ + 1, originTerritory_->getUnitCount() - 1 - originTerritory_->getMovedUnitsCount());
			}
		}
	}

	// DOWN
	if (inputManager_->isKeyPressed(SDLK_DOWN)) {
		// if attacker and defender are chosen, decrease troops to attack
		if (originTerritory_ != nullptr && destinationTerritory_ != nullptr) {
			// if shift is pushed substract 5 or set to 0
			if (inputManager_->isKeyDown(SDLK_LSHIFT)) {
				movingUnits_ = std::max(movingUnits_ - 5 , 0);
			}
			// if attacking units are greater than 0, decrement
			movingUnits_ = std::max(movingUnits_ - 1, 0);
		}
	}
}


void StatePhase6::drawGame() {

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
	
	
	// STATIONARY (ON THE SCREEN)
	// set the stationary camera matrix
	cameraMatrix = hudCamera2D_->getCameraMatrix();
	// upload matrix to the gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	territoryBatch_->begin();
	// if activated draw the dice
	if (diceActivatited_) {
		if (movingUnits_ > 0) {
			// move into defending territory
			// set new owner
			destinationTerritory_->setOwner(currentPlayer_);
			for (int i = 0; i < movingUnits_--;) {
				// add units to invaded territory
				destinationTerritory_->addUnit(audioEngine_);
				destinationTerritory_->setMovedUnitsCount(destinationTerritory_->getMovedUnitsCount() + 1);
				// substract units from attacking territory
				originTerritory_->destroyUnit(audioEngine_);
			}
		}
		// deselect the attacker territory and its neighbours
		for (auto territory : territories_) {
			territory->lightDownTerritory(true);
		}
		// reset color of attacker and defender territories
		originTerritory_->resetColor();
		if (destinationTerritory_ != nullptr) {
			destinationTerritory_->resetColor();
		}
		// unmark the attacker and defender territory
		originTerritory_ = nullptr;
		destinationTerritory_ = nullptr;
		// deactivate the fight
		diceActivatited_ = false;
		// make ready for going to next state
		leaveOK_ = true;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// show whos turn it is
	drawHud(currentPlayer_->getName() + "'s turn!", glm::vec2(-750, 390), glm::vec2(standardFontSize_), currentPlayer_->getColor(), true);
	// draw the hud
	drawHud("Phase 6: Move troops!", glm::vec2(-750, 360), glm::vec2(standardFontSize_), Bengine::ColorRGBA8(255, 255, 255, 255), true);
	if (originTerritory_ != nullptr && destinationTerritory_ != nullptr) {
		// draw the number of moving units if origin and destination are chosen
		drawHud(("Moving: " + std::to_string(movingUnits_)),
			glm::vec2(originTerritory_->getPosition().x + originTerritory_->getPosition().z * 0.8, originTerritory_->getPosition().y + originTerritory_->getPosition().w * 0.8),
			glm::vec2(0.5),
			Bengine::ColorRGBA8(255, 255, 255, 255),
			false);
	}


	// disable the shader
	colorProgram_->unuse();

	// setup our buffer and draw everything to the screen
	window_->swapBuffer();
}