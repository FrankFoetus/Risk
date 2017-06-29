#pragma once

#include <string>
#include <iostream>

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <Bengine\Vertex.h>
#include <Bengine\SpriteBatch.h>

#include "Player.h"
#include "Continent.h"
#include "Unit.h"

class Player;
class Continent;

class Territory
{
public:
	Territory(std::string name, glm::vec4 position, glm::vec4 uv, GLuint textureID, Bengine::ColorRGBA8 color);
	~Territory();

	std::string getName() { return name_; }
	glm::vec4 getPosition() { return position_; }
	glm::vec4 getUV() { return uv_; }
	Bengine::ColorRGBA8 getColor() { return color_; }
	GLuint getTextureID() { return textureID_; }

	Player* getOwner() { return owner_; }
	Continent* getContinent() { return continent_; }
	std::vector<Unit*> getUnitsT1() { return unitsT1_; }
	std::vector<Unit*> getUnitsT2() { return unitsT2_; }
	std::vector<Unit*> getUnitsT3() { return unitsT3_; }
	int getUnitCount() { return unitCount_; }

	void setPosition(glm::vec4 position) { position_ = position; }
	void setUV(glm::vec4 uv) { uv_ = uv; }
	void setColor(Bengine::ColorRGBA8 color) { color_ = color; }
	void setTextureID(GLuint textureID) { textureID_ = textureID; }

	void setOwner(Player* player) { owner_ = player; }
	void setContinent(Continent* continent) { continent_ = continent; }
	void setUnitsT1(std::vector<Unit*> unitsT1) { unitsT1_ = unitsT1; }
	void setUnitsT2(std::vector<Unit*> unitsT2) { unitsT2_ = unitsT2; }
	void setUnitsT3(std::vector<Unit*> unitsT3) { unitsT3_ = unitsT3; }
	void setUnitCount(int count) { unitCount_ = count; }

	void addUnit(Bengine::AudioEngine* audioEngine);
	void destroyUnit(Bengine::AudioEngine* audioEngine);
	void lightUpTerritory();
	void lightDownTerritory();

	void resetColor() { color_ = origColor_; }

	void update();
	void draw(Bengine::SpriteBatch* territoryBatch);

private:
	void unitTransformation(Bengine::AudioEngine* audioEngine, int Tx, int Ty);

	void drawUnits(Bengine::SpriteBatch* territoryBatch);

	std::string name_;

	glm::vec4 position_;
	glm::vec4 uv_;
	Bengine::ColorRGBA8 color_;
	Bengine::ColorRGBA8 origColor_;
	GLuint textureID_;

	Continent* continent_ = nullptr;
	Player* owner_ = nullptr;
	std::vector<Unit*> unitsT1_;
	std::vector<Unit*> unitsT2_;
	std::vector<Unit*> unitsT3_;
	int unitCount_ = 0;
};

