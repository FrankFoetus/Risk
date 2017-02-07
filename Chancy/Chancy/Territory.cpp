#include "Territory.h"



Territory::Territory(std::string name, glm::vec4 position, glm::vec4 uv, GLuint textureID, Bengine::ColorRGBA8 color) :
		name_(name), position_(position), uv_(uv), textureID_(textureID), color_(color)
{
}


Territory::~Territory()
{
}


void Territory::draw(Bengine::SpriteBatch* territoryBatch, Bengine::SpriteBatch* unitsT1Batch, Bengine::SpriteBatch* unitsT2Batch, Bengine::SpriteBatch* unitsT3Batch) {
	territoryBatch->draw(position_, uv_, textureID_, 0, color_);
	drawUnits(unitsT1Batch, unitsT2Batch, unitsT3Batch);
}


void Territory::drawUnits(Bengine::SpriteBatch* unitsT1Batch, Bengine::SpriteBatch* unitsT2Batch, Bengine::SpriteBatch* unitsT3Batch) {
	for (auto it : unitsT1_) {
		it->draw(unitsT1Batch);
	}
	for (auto it : unitsT2_) {
		it->draw(unitsT2Batch);
	}
	for (auto it : unitsT3_) {
		it->draw(unitsT3Batch);
	}
}


void Territory::update() {

}
