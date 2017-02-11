#include "Unit.h"



Unit::Unit(glm::vec4 position, glm::vec4 uv, int textureID, 
	Bengine::ColorRGBA8 color, Bengine::SoundEffect spawnSoundEffect) :
	position_(position), uv_(uv), textureID_(textureID), color_(color), spawnSoundEffect_(spawnSoundEffect)
{
}


Unit::Unit() 
{
	// empty default constructor
}

Unit::~Unit()
{
	// empty default deconstructor
}


void Unit::draw(Bengine::SpriteBatch* spriteBatch) {
	// draw the unit to the unit sprite batch
	spriteBatch->draw(position_, uv_, textureID_, 0, color_);
}