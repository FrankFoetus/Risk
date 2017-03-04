#pragma once

#include <string>

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <Bengine\Vertex.h>
#include <Bengine\SpriteBatch.h>

#include "Player.h"
#include "Continent.h"
#include "UnitT1.h"
#include "UnitT2.h"
#include "UnitT3.h"

class Player;

class Territory
{
public:
	Territory(std::string name, glm::vec4 position, glm::vec4 uv, GLuint textureID, Bengine::ColorRGBA8 color);
	~Territory();

	glm::vec4 getPosition() { return position_; }
	glm::vec4 getUV() { return uv_; }
	Bengine::ColorRGBA8 getColor() { return color_; }
	GLuint getTextureID() { return textureID_; }

	Player* getOwner() { return owner_; }
	std::vector<UnitT1*> getUnitsT1() { return unitsT1_; }
	std::vector<UnitT2*> getUnitsT2() { return unitsT2_; }
	std::vector<UnitT3*> getUnitsT3() { return unitsT3_; }

	void setPosition(glm::vec4 position) { position_ = position; }
	void setUV(glm::vec4 uv) { uv_ = uv; }
	void setColor(Bengine::ColorRGBA8 color) { color_ = color; }
	void setTextureID(GLuint textureID) { textureID_ = textureID; }

	void setOwner(Player* player) { owner_ = player; }
	void setUnitsT1(std::vector<UnitT1*> unitsT1) { unitsT1_ = unitsT1; }
	void setUnitsT2(std::vector<UnitT2*> unitsT2) { unitsT2_ = unitsT2; }
	void setUnitsT3(std::vector<UnitT3*> unitsT3) { unitsT3_ = unitsT3; }

	void draw(Bengine::SpriteBatch* territoryBatch);

	void update();

private:
	void drawUnits(Bengine::SpriteBatch* territoryBatch);

	std::string name_;

	glm::vec4 position_;
	glm::vec4 uv_;
	Bengine::ColorRGBA8 color_;
	GLuint textureID_;

	Player* owner_ = nullptr;
	std::vector<UnitT1*> unitsT1_;
	std::vector<UnitT2*> unitsT2_;
	std::vector<UnitT3*> unitsT3_;
};

