#include "Dice.h"


Dice::Dice() {
	// default adds 3 dice

	/* initialize random seed: */
	srand(time(NULL));
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceOne.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceTwo.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceThree.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceFour.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceFive.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceSix.png").id);

	color_.push_back(Bengine::ColorRGBA8(255, 255, 255, 255));
	color_.push_back(Bengine::ColorRGBA8(255, 255, 255, 255));
	color_.push_back(Bengine::ColorRGBA8(255, 255, 255, 255));

	position_.emplace_back(glm::vec4(-1 * dieSize_ * 1.5 - 10, dieSize_ + 10, dieSize_, dieSize_));
	position_.emplace_back(glm::vec4(-1 * dieSize_ * 0.5, dieSize_  + 10, dieSize_, dieSize_));
	position_.emplace_back(glm::vec4(dieSize_ * 1.5 + 10, dieSize_ + 10, dieSize_, dieSize_));
}



Dice::Dice(int numberOfDice, glm::vec2 upperLeftCornerPos, GLuint dieSize) : dieSize_(dieSize)
{
	/* initialize random seed: */
	srand(time(NULL));
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceOne.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceTwo.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceThree.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceFour.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceFive.png").id);
	textureIDs_.push_back(Bengine::RessourceManager::getTexture("Textures/NapoleonsDice/diceSix.png").id);

	for (int i = 0; i < numberOfDice; i++) {
		// push back the color
		color_.push_back(Bengine::ColorRGBA8(255, 255, 255, 255));
		// push back the postion
		position_.emplace_back(glm::vec4(upperLeftCornerPos.x + (dieSize_ + 10) * i, upperLeftCornerPos.y - dieSize_, dieSize_, dieSize_));
	}
}


Dice::~Dice() {
}


void Dice::roll(int numberOfDice) {
	currentRoll_.clear();
	for (int i = 0; i < numberOfDice; i++) {
		currentRoll_.push_back(rand() % 6 + 1);
	}
}


void Dice::draw(Bengine::SpriteBatch* spriteBatch) {
	for (int i = 0; i < currentRoll_.size(); i++) {
		spriteBatch->draw(position_[i], glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), textureIDs_[currentRoll_[i] - 1], 0, color_[i]);
	}
}