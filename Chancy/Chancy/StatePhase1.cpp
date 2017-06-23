#include "StatePhase1.h"



StatePhase1::StatePhase1()
{
	//empty
}


StatePhase1::~StatePhase1()
{
	//empty
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

	// process escape push
	if (inputManager_->isKeyPressed(SDLK_ESCAPE)) {
		// go into menu
		gameState = GameState::MENU;
		std::cout << "ENTERING MENU" << std::endl;
	}

	// process left mouse click
	if (inputManager_->isKeyPressed(SDL_BUTTON_LEFT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_->convertScreenToWorld(inputManager_->getMouseCoords()));
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
	if (inputManager_->isKeyPressed(SDL_BUTTON_RIGHT)) {
		// check if territory was hit by click
		Territory* territory = checkDistanceToTerritory(camera2D_->convertScreenToWorld(inputManager_->getMouseCoords()));
		if (territory != NULL) {
			// if left control is pressed, remove units
			if (inputManager_->isKeyDown(SDLK_LCTRL)) {
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
					}
					else {
						unitTransformation(territory, 2, 1);
					}
				}
				else {
					unitTransformation(territory, 1, 0);
				}
				// if left control is not pressed, add units
			}
			else {
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

	// draw the hud
	int numberOfReinforcements = 5;
	drawHud("Phase 1: Place your reinforcements! " + std::to_string(numberOfReinforcements) + " left.", glm::vec2(-700, 360), glm::vec2(2), Bengine::ColorRGBA8(255, 255, 255, 255));

	// disable the shader
	colorProgram_->unuse();

	// setup our buffer and draw everything to the screen
	window_->swapBuffer();
}