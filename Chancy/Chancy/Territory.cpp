#include "Territory.h"



Territory::Territory(std::string name, glm::vec4 position, glm::vec4 uv, GLuint textureID, Bengine::ColorRGBA8 color) :
		name_(name), position_(position), uv_(uv), textureID_(textureID), color_(color)
{
}


Territory::~Territory()
{
}


void Territory::draw(Bengine::SpriteBatch* spriteBatch) {
	spriteBatch->draw(position_, uv_, textureID_, 0, color_);
	drawUnits(spriteBatch);
}


void Territory::drawUnits(Bengine::SpriteBatch* spriteBatch) {
	for (auto it : unitsT1_) {
		it->draw(spriteBatch);
	}
	for (auto it : unitsT2_) {
		it->draw(spriteBatch);
	}
	for (auto it : unitsT3_) {
		it->draw(spriteBatch);
	}
}


void Territory::update() {

}
