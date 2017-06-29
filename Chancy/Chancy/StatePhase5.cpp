#include "StatePhase5.h"



StatePhase5::StatePhase5()
{
}


StatePhase5::~StatePhase5()
{
}


void StatePhase5::enterState(int currentPlayer) {
	if (!enteredState_) {
		std::cout << "ENTERING STATE 5" << std::endl;
		// set the current Player
		currentPlayer_ = players_[currentPlayer];
		// light down all territories
		for (auto terr : territories_) {
			terr->lightDownTerritory();
		}
		// dont enter state again before next round
		enteredState_ = true;
	}
}


void StatePhase5::processInput(float* cameraSpeed, float* scaleSpeed, GameState& gameState) {

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

	// P
	if (inputManager_->isKeyPressed(SDLK_p)) {
		
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
			// if territory was not lid up, light it and its neighbours up and all other territories down 
			if (territory->getColor().a < 255) {
				if (territory->getOwner() == currentPlayer_) {
					// CHANGING ATTACKER:
					// light down all territories
					for (auto terr : territories_) {
						terr->lightDownTerritory();
					}
					if (attackerTerritory_ != nullptr) {
						// reset color of attacker territory
						attackerTerritory_->resetColor();
					}
					// mark the selected territory as the attacker
					attackerTerritory_ = territory;
					// adjust color of attacker territory
					attackerTerritory_->setColor(Bengine::ColorRGBA8(0, 0, 0, 255));
					// reset the defender
					if (defenderTerritory_ != nullptr) {
						defenderTerritory_->resetColor();
						defenderTerritory_ = nullptr;
					}

					// light up attacker territory and its neighbours
					attackerTerritory_->lightUpTerritory();
					for (auto terr : getNeighbours(attackerTerritory_)) {
						// only light up territories of other players that can be attacked
						if (terr->getOwner() != currentPlayer_) {
							terr->lightUpTerritory();
						}
					}
				}
				else {
					std::cout << "This territory does not belong to You!" << std::endl;
				}
			}
			// if territory is already lid up
			else {
				if (territory == attackerTerritory_) {
					// deselect the territory and its neighbours
					for (auto territory : territories_) {
						territory->lightDownTerritory();
					}
					// reset color of attacker and defender territories
					attackerTerritory_->resetColor();
					if (defenderTerritory_ != nullptr) {
						defenderTerritory_->resetColor();
					}
					// unmark the attacker and defender territory
					attackerTerritory_ = nullptr;
					defenderTerritory_ = nullptr;
				}
				else {
					if (defenderTerritory_ != nullptr) {
						// reset the color of the defender territorie
						defenderTerritory_->resetColor();
					}
					// choose number of units to attack
					defenderTerritory_ = territory;
					// adjust color of defender territories
					defenderTerritory_->setColor(Bengine::ColorRGBA8(0, 0, 0, 255));
				}
			}
		}
	}
}


void StatePhase5::drawGame() {

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
	for (auto territory : territories_) {
		// draw (add) all territories to the sprite batch
		territory->draw(territoryBatch_);
	}
	territoryBatch_->end();
	// render the territories
	territoryBatch_->renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// draw the hud
	drawHud("Phase 5: Invade territories!", glm::vec2(-750, 360), glm::vec2(2), Bengine::ColorRGBA8(255, 255, 255, 255));

	// disable the shader
	colorProgram_->unuse();

	// setup our buffer and draw everything to the screen
	window_->swapBuffer();
}


std::vector<Territory*> StatePhase5::getNeighbours(Territory* territory) {
	std::vector<Territory*> neighbours;
	for (auto terr : territories_) {
		// calculate the distance between the two territories
		float distance = std::sqrt((terr->getPosition().x - territory->getPosition().x) * (terr->getPosition().x - territory->getPosition().x) +
						(terr->getPosition().y - territory->getPosition().y) * (terr->getPosition().y - territory->getPosition().y));
		// if the distance is small enough, add the terr to neighbours
		if (distance < 1.5*territory->getPosition().w) {
			neighbours.push_back(terr);
		}
	}
	return neighbours;
}


void StatePhase5::leaveState(GameState& gameState) {
	// go to the next phase
	gameState = GameState::PHASE6;
	std::cout << "ENTERING PHASE 6" << std::endl;
	// make state enterable again
	enteredState_ = false;
}