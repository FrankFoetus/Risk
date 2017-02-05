#pragma once

#include <string>
#include <vector>

#include <Bengine\SpriteBatch.h>
#include <Bengine\RessourceManager.h>

#include "Continent.h"


const float TILE_WIDTH = 128.0f;

class Level
{
public:
	// load the level
	Level(const std::string fileName, GLuint windowHeight_, GLuint windowWidth_);
	~Level();

	// getters
	int getWidth() const { return levelData_[0].size(); }
	int getHeight() const { return levelData_.size(); }

	std::vector<Territory*>& getTerritories() { return territories_; }
	const std::vector<std::string>& getLevelData() { return levelData_; }

private:
	std::vector<std::string> levelData_;

	Territory* territory_ = nullptr;

	std::vector<Territory*> territories_;
	std::vector<Continent*> continents_;

	std::vector<std::string> territoryNames_ = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
};
