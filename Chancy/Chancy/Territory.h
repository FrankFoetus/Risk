#pragma once

#include <string>

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <Bengine\Vertex.h>

#include "Continent.h"

class Territory
{
public:
	Territory(std::string name, glm::vec4 position, glm::vec4 uv, GLuint textureID, Bengine::ColorRGBA8 color);
	~Territory();

	void init();

	glm::vec4 getPosition() { return position_; }
	glm::vec4 getUV() { return uv_; }
	Bengine::ColorRGBA8 getColor() { return color_; }
	GLuint getTextureID() { return textureID_; }

	void setPosition(glm::vec4 position) { position_ = position; }
	void setUV(glm::vec4 uv) { uv_ = uv; }
	void setColor(Bengine::ColorRGBA8 color) { color_ = color; }
	void setTextureID(GLuint textureID) { textureID_ = textureID; }

private:
	std::string name_;

	glm::vec4 position_;
	glm::vec4 uv_;
	Bengine::ColorRGBA8 color_;
	GLuint textureID_;
};

