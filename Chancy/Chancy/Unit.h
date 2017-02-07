#pragma once

#include <GL\glew.h>
#include <Bengine\SpriteBatch.h>
#include <Bengine\RessourceManager.h>

class Unit
{
public:
	Unit(glm::vec4 position, glm::vec4 uv, int textureID, Bengine::ColorRGBA8 color);
	Unit();
	~Unit();

	glm::vec4 getPosition() { return position_; }
	glm::vec4 getUV() { return uv_; }
	Bengine::ColorRGBA8 getColor() { return color_; }
	GLuint getTextureID() { return textureID_; }

	void setPosition(glm::vec4 position) { position_ = position; }
	void setUV(glm::vec4 uv) { uv_ = uv; }
	void setColor(Bengine::ColorRGBA8 color) { color_ = color; }
	void setTextureID(GLuint textureID) { textureID_ = textureID; }

	// draw the unit to the unit sprite batch
	void draw(Bengine::SpriteBatch* spriteBatch);

private:
	glm::vec4 position_;
	glm::vec4 uv_;
	Bengine::ColorRGBA8 color_;
	GLuint textureID_;
};

