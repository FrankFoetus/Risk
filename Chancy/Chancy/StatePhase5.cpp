#include "StatePhase5.h"



StatePhase5::StatePhase5()
{
}


StatePhase5::~StatePhase5()
{
}


void StatePhase5::enterState(int playerIndex) {
	if (!enteredState_) {
		std::cout << "ENTERING STATE 5" << std::endl;
		// set the current Player
		currentPlayer_ = players_[playerIndex];
		// light down all territories
		for (auto terr : territories_) {
			terr->lightDownTerritory();
		}
		// dont enter state again before next round
		enteredState_ = true;
	}
}


void StatePhase5::leaveState(GameState& gameState) {
	// go to the next phase
	gameState = GameState::PHASE6;
	std::cout << "ENTERING PHASE 6" << std::endl;
	// make state enterable again
	enteredState_ = false;
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

	// only process inputs if dice are inactive
	if (!diceActivatited_) {
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
			if (attackingUnits_ > 0) {
				// process invasion
				diceActivatited_ = true;
				// roll the dice once
				attackerDice_->roll(std::min(attackingUnits_, 3));
				defenderDice_->roll(std::min(defendingUnits_, 2));
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
						attackingUnits_ = 0;
						// adjust color and size of attacker territory
						attackerTerritory_->setColor(Bengine::ColorRGBA8(255, 255, 255, 255));
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
						// make ready for going to next state
						leaveOK_ = true;
					}
					// if selected territory is lid up and not the attacker territory
					else {
						if (defenderTerritory_ != nullptr) {
							// reset the color of the defender territorie
							defenderTerritory_->resetColor();
							defenderTerritory_->lightUpTerritory();
						}
						// choose number of units to attack
						defenderTerritory_ = territory;
						attackingUnits_ = 0;
						// adjust color of defender territories
						defenderTerritory_->setColor(Bengine::ColorRGBA8(255, 255, 255, 255));
						// set defending units count
						defendingUnits_ = defenderTerritory_->getUnitCount();
					}
				}
			}
		}

		// UP
		if (inputManager_->isKeyPressed(SDLK_UP)) {
			// if attacker and defender are chosen, count up troops to attack
			if (attackerTerritory_ != nullptr && defenderTerritory_ != nullptr) {
				// if shift is pushed add 5 or maximum units in territory
				if (inputManager_->isKeyDown(SDLK_LSHIFT)) {
					attackingUnits_ = std::min(attackingUnits_ + 5, attackerTerritory_->getUnitCount() - 1);
				}
				// if attacking units are less than units in attacking territory, increment
				attackingUnits_ = std::min(attackingUnits_ + 1, attackerTerritory_->getUnitCount() - 1);
			}
		}

		// DOWN
		if (inputManager_->isKeyPressed(SDLK_DOWN)) {
			// if attacker and defender are chosen, decrease troops to attack
			if (attackerTerritory_ != nullptr && defenderTerritory_ != nullptr) {
				// if shift is pushed substract 5 or set to 0
				if (inputManager_->isKeyDown(SDLK_LSHIFT)) {
					attackingUnits_ = std::max(attackingUnits_ - 5, 0);
				}
				// if attacking units are greater than 0, decrement
				attackingUnits_ = std::max(attackingUnits_ - 1, 0);
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

	// NON STATIONARY (ON THE SCREEN)
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

	// STATIONARY (ON THE SCREEN)
	// set the stationary camera matrix
	cameraMatrix = hudCamera2D_->getCameraMatrix();
	// upload matrix to the gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	territoryBatch_->begin();
	// if activated draw the dice
	if (diceActivatited_) {
		if (waitTime_ < 180) { // wait 3 seconds (with 60 fps)
			attackerDice_->draw(territoryBatch_);
			defenderDice_->draw(territoryBatch_);
			// dont wait when there are no defending units
			if (defendingUnits_ == 0) {
				waitTime_ = 180;
			}
			waitTime_++;
		}
		else {
			if (attackingUnits_ > 0 && defendingUnits_ > 0) {
				// ATTACK!
				// calculate winners
				processBattle();
				if (attackingUnits_ > 0) {
					waitTime_ = 0;
					// roll the dice
					attackerDice_->roll(std::min(attackingUnits_, 3));
					defenderDice_->roll(std::min(defendingUnits_, 2));
				}
			}
			else {
				if (attackingUnits_ > 0) {
					// move into defending territory
					// set new owner
					defenderTerritory_->setOwner(currentPlayer_);
					for (int i = 0; i < attackingUnits_--;) {
						// add units to invaded territory
						defenderTerritory_->addUnit(audioEngine_);
						// substract units from attacking territory
						attackerTerritory_->destroyUnit(audioEngine_);
					}
				}
				// deselect the attacker territory and its neighbours
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
				// deactivate the fight
				diceActivatited_ = false;
				// make ready for going to next state
				leaveOK_ = true;
			}
		}
	}
	territoryBatch_->end();
	territoryBatch_->renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// show whos turn it is
	drawHud(currentPlayer_->getName() + "'s turn!", glm::vec2(-750, 390), glm::vec2(standardFontSize_), currentPlayer_->getColor(), true);
	// draw the head line
	drawHud("Phase 5: Invade territories!", glm::vec2(-750, 360), glm::vec2(standardFontSize_), Bengine::ColorRGBA8(255, 255, 255, 255), true);
	if (attackerTerritory_ != nullptr && defenderTerritory_ != nullptr) {
		// draw the number of attacking units if attacker and defender are chosen
		drawHud(("Attackers: " + std::to_string(attackingUnits_)),
			glm::vec2(attackerTerritory_->getPosition().x + attackerTerritory_->getPosition().z * 0.8, attackerTerritory_->getPosition().y + attackerTerritory_->getPosition().w * 0.8),
			glm::vec2(0.5),
			Bengine::ColorRGBA8(255, 255, 255, 255),
			false);
		// draw the number of attacking units if attacker and defender are chosen
		drawHud(("Defenders: " + std::to_string(defendingUnits_)),
			glm::vec2(defenderTerritory_->getPosition().x + defenderTerritory_->getPosition().z * 0.8, defenderTerritory_->getPosition().y + defenderTerritory_->getPosition().w * 0.8),
			glm::vec2(0.5),
			Bengine::ColorRGBA8(255, 255, 255, 255),
			false);
	}

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


void StatePhase5::processBattle() {
	// sort the roll results
	std::vector<int> attackResults = attackerDice_->getRoll();
	std::sort(attackResults.begin(), attackResults.end());
	std::reverse(attackResults.begin(), attackResults.end());
	std::vector<int> defendResults = defenderDice_->getRoll();
	std::sort(defendResults.begin(), defendResults.end());
	std::reverse(defendResults.begin(), defendResults.end());
	// compare every activ attack die with every active defend die
	for (int i = 0; i < std::min(attackResults.size(), defendResults.size()); i++) {
		if (attackResults[i] > defendResults[i]) {
			// destroy lost defending units
			defenderTerritory_->destroyUnit(audioEngine_);
			defendingUnits_--;
		}
		else {
			// destroy lost attacking units
			attackerTerritory_->destroyUnit(audioEngine_);
			attackingUnits_--;
		}
	}
}