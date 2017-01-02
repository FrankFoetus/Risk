#pragma once

#include <string>
#include <vector>

#include <Bengine\SpriteBatch.h>
#include <Bengine\RessourceManager.h>


const float TILE_WIDTH = 64.0f;

class Level
{
public:
	// load the level
	Level(const std::string fileName, GLuint windowHeight_, GLuint windowWidth_);
	~Level();

	void draw();

	// getters
	int getWidth() const { return levelData_[0].size(); }
	int getHeight() const { return levelData_.size(); }
	const std::vector<std::string>& getLevelData() { return levelData_; }

private:
	std::vector<std::string> levelData_;
	Bengine::SpriteBatch spriteBatch_;

};
