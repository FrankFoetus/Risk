#pragma once

#include <GL\glew.h>
#include <Bengine\SpriteBatch.h>
#include <Bengine\RessourceManager.h>
#include <Bengine\AudioEngine.h>

class Unit
{
public:
	Unit(glm::vec4 position, glm::vec4 uv, int textureID, 
		Bengine::ColorRGBA8 color, Bengine::SoundEffect spawnSoundEffect);
	Unit();
	~Unit();

	// getters
	glm::vec4 getPosition() { return position_; }
	glm::vec4 getUV() { return uv_; }
	Bengine::ColorRGBA8 getColor() { return color_; }
	GLuint getTextureID() { return textureID_; }
	Bengine::SoundEffect getSpawnSoundEffect() { return spawnSoundEffect_; }
	bool getHasMoved() { return hasMoved_; }

	// setters
	void setPosition(glm::vec4 position) { position_ = position; }
	void setUV(glm::vec4 uv) { uv_ = uv; }
	void setColor(Bengine::ColorRGBA8 color) { color_ = color; }
	void setTextureID(GLuint textureID) { textureID_ = textureID; }
	void setSpawnSOundEffect(Bengine::SoundEffect spawnSoundEffect) { spawnSoundEffect_ = spawnSoundEffect; }
	void setHasMoved(bool hasMoved) { hasMoved_ = hasMoved; }

	// draw the unit to the unit sprite batch
	void draw(Bengine::SpriteBatch* spriteBatch);

protected:

	Bengine::SoundEffect spawnSoundEffect_;

	glm::vec4 position_;
	glm::vec4 uv_;
	Bengine::ColorRGBA8 color_;
	GLuint textureID_;
	bool hasMoved_ = false;
};

