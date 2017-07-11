#include "State.h"



State::State()
{
}


State::~State()
{
}

void State::init(Bengine::InputManager* inputManager, Bengine::Camera2D* camera2D, Bengine::Camera2D* hudCamera2D, Bengine::AudioEngine* audioEngine, Level* level, 
	Bengine::GLSLProgram* colorProgram, Bengine::SpriteFont* spriteFont, Bengine::SpriteBatch* territoryBatch, Bengine::SpriteBatch* hudSpriteBatch, 
	Bengine::Window* window, Dice* attackerDice, Dice* defenderDice, const GLuint windowWidth, const GLuint windowHeight, std::vector<Player*> players, std::vector<Continent*> continents) {
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
	players_ = players;
	continents_ = continents;
	attackerDice_ = attackerDice;
	defenderDice_ = defenderDice;

	territories_ = level->getTerritories();
	/* debug output the territory names
	for (auto it : territories_) {
		std::cout << it->getName() << std::endl;
	}
	*/
}


void State::enterState(int playerIndex) {

	if (!enteredState_) {
		// set the current Player
		currentPlayer_ = players_[playerIndex];
		// dont enter state again before next round
		enteredState_ = true;
	}
}


void State::leaveState(GameState& gameState) {
	enteredState_ = false;
}


void State::drawHud(std::string text, glm::vec2 pos, glm::vec2 size, Bengine::ColorRGBA8 color, bool stationary) {
	// set the camera matrix
	glm::mat4 cameraMatrix;
	if (stationary) {
		cameraMatrix = hudCamera2D_->getCameraMatrix();
	}
	else
	{
		cameraMatrix = camera2D_->getCameraMatrix();
	}
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



int State::calculateReinforcements() {
	// initialize #reinforcements
	int numberOfReinforcements = 0;
	// add 1 reinforcement for every owned territory
	for (auto territory : territories_) {
		if (territory->getOwner() == currentPlayer_) {
			numberOfReinforcements++;
		}
	}
	// one troop for every three territories
	numberOfReinforcements /= 3;
	// at least 3 troops for territories
	if (numberOfReinforcements < 3) {
		numberOfReinforcements = 3;
	}
	// add continental boni
	for (auto continent : continents_) {
		if (continent->getOwner() == currentPlayer_) {
			numberOfReinforcements += continent->getBonus();
		}
	}
	return numberOfReinforcements;
}


std::vector<Territory*> State::getNeighbours(Territory* territory) {
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