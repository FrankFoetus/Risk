#include "State.h"



State::State()
{
}


State::~State()
{
}

void State::init(Bengine::InputManager* inputManager, Bengine::Camera2D* camera2D, Bengine::Camera2D* hudCamera2D, Bengine::AudioEngine* audioEngine, Level* level, Bengine::GLSLProgram* colorProgram,
	Bengine::SpriteFont* spriteFont, Bengine::SpriteBatch* territoryBatch, Bengine::SpriteBatch* hudSpriteBatch, Bengine::Window* window, const GLuint windowWidth, const GLuint windowHeight) {
	inputManager_ = inputManager;
	camera2D_ = camera2D;
	hudCamera2D_ = hudCamera2D;
	audioEngine_ = audioEngine;
	level_ = level;
	window_ = window;
	windowWidth_ = windowWidth;
	windowHeight_ = windowHeight;
	colorProgram_ = colorProgram;
	spriteFont_ = spriteFont;
	territoryBatch_ = territoryBatch;
	hudSpritebatch_ = hudSpriteBatch;
	enteredState_ = false;

	territories_ = level->getTerritories();
	for (auto it : territories_) {
		std::cout << it->getName() << std::endl;
	}
}


void State::enterState(Player* currentPlayer) {

	if (!enteredState_) {
		// set the current Player
		currentPlayer_ = currentPlayer;
		// dont enter state again before next round
		enteredState_ = true;
	}
}


void State::leaveState(GameState& gameState) {
	enteredState_ = false;
}


void State::drawHud(std::string text, glm::vec2 pos, glm::vec2 size, Bengine::ColorRGBA8 color) {
	// set the camera matrix
	glm::mat4 cameraMatrix = hudCamera2D_->getCameraMatrix();
	GLint pLocation = colorProgram_->getUniformLocation("P");
	// upload matrix to the gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	char buffer[256];

	hudSpritebatch_->begin();

	sprintf_s(buffer, text.c_str());
	spriteFont_->draw(*hudSpritebatch_, buffer, pos, size, 0.0f, color);

	hudSpritebatch_->end();
	hudSpritebatch_->renderBatch();
}



/**
checks if there are any territories hit by mouse click
*/
Territory* State::checkDistanceToTerritory(glm::vec2 mousePos) {
	for (auto it : level_->getTerritories()) {
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

