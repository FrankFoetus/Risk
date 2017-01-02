#include "Level.h"

#include <Bengine/Error.h>

#include <fstream>


Level::Level(const std::string fileName, GLuint windowHeight_, GLuint windowWidth_) {
	// open the file
	std::ifstream file;
	file.open(fileName);

	// error checking
	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}

	// tmp
	std::string tmp;

	while (std::getline(file, tmp)) {
		levelData_.push_back(tmp);
	}

	spriteBatch_.init();
	spriteBatch_.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	// render all tiles
	for (GLuint y = 0; y < levelData_.size(); y++) {
		for (GLuint x = 0; x < levelData_[y].size(); x++) {
			// grab the tile
			char tile = levelData_[y][x];

			// get dest rect
			glm::vec4 destRect(-double(windowWidth_/2)+x*TILE_WIDTH, -double(windowHeight_/2)+y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			// process the tile
			switch (tile) {
			case 'R':
				spriteBatch_.draw(destRect,
					uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_OrangeDark.png").id,
					0,
					Bengine::ColorRGBA8(255, 255, 255, 255));
				break;
			case 'G':
				spriteBatch_.draw(destRect,
					uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_Blue.png").id,
					0,
					Bengine::ColorRGBA8(255, 255, 255, 255));
				break;
			case 'L':
				spriteBatch_.draw(destRect,
					uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_BeigheDark.png").id,
					0,
					Bengine::ColorRGBA8(255, 255, 255, 255));
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}

	spriteBatch_.end();
}


Level::~Level()
{
	// empty deconstructor
}


void Level::draw() {
	spriteBatch_.renderBatch();
}
