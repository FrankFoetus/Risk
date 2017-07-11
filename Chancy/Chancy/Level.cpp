#include "Level.h"
#include "Territory.h"

#include <Bengine/BengineError.h>

#include <fstream>
#include <iostream>


Level::Level(const std::string fileName, GLuint windowHeight_, GLuint windowWidth_) {
	// open the file
	std::ifstream file;
	file.open(fileName);

	// error checking
	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}

	// create the continents
	continents_.push_back(new Continent("Temeria", 2));
	continents_.push_back(new Continent("Scellige", 3));
	continents_.push_back(new Continent("Nilfgard", 4));
	continents_.push_back(new Continent("Kaer Morhen", 5));
	continents_.push_back(new Continent("Velen", 6));
	continents_.push_back(new Continent("Vizima", 7));

	// tmporal variable to load data from text file
	std::string tmp;

	while (std::getline(file, tmp)) {
		levelData_.push_back(tmp);
	}

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	// index for territory names
	unsigned int ind = 0;

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
				territory_ = new Territory(territoryNames_[ind++], destRect, uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_BeigheDark.png").id,
					Bengine::ColorRGBA8(255, 0, 0, 150));
				// set the continent for the territory
				territory_->setContinent(continents_[0]);
				continents_[0]->setNumberOfTerritories(continents_[0]->getNumberOfTerritories() + 1);
				// place the new territory into all territories vector
				territories_.push_back(territory_);
				break;
			case 'G':
				territory_ = new Territory(territoryNames_[ind++], destRect, uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_BeigheDark.png").id,
					Bengine::ColorRGBA8(0, 255, 0, 150));
				// set the continent for the territory
				territory_->setContinent(continents_[1]);
				continents_[1]->setNumberOfTerritories(continents_[1]->getNumberOfTerritories() + 1);
				// place the new territory into all territories vector
				territories_.push_back(territory_);
				break;
			case 'B':
				territory_ = new Territory(territoryNames_[ind++], destRect, uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_BeigheDark.png").id,
					Bengine::ColorRGBA8(0, 0, 255, 150));
				// set the continent for the territory
				territory_->setContinent(continents_[2]);
				continents_[2]->setNumberOfTerritories(continents_[2]->getNumberOfTerritories() + 1);
				// place the new territory into all territories vector
				territories_.push_back(territory_);
				break;
			case 'Y':
				territory_ = new Territory(territoryNames_[ind++], destRect, uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_BeigheDark.png").id,
					Bengine::ColorRGBA8(0, 255, 255, 150));
				// set the continent for the territory
				territory_->setContinent(continents_[3]);
				continents_[3]->setNumberOfTerritories(continents_[3]->getNumberOfTerritories() + 1);
				// place the new territory into all territories vector
				territories_.push_back(territory_);
				break;
			case 'M':
				territory_ = new Territory(territoryNames_[ind++], destRect, uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_BeigheDark.png").id,
					Bengine::ColorRGBA8(255, 0, 255, 150));
				// set the continent for the territory
				territory_->setContinent(continents_[4]);
				continents_[4]->setNumberOfTerritories(continents_[4]->getNumberOfTerritories() + 1);
				// place the new territory into all territories vector
				territories_.push_back(territory_);
				break;
			case 'C':
				territory_ = new Territory(territoryNames_[ind++], destRect, uvRect,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Buttons/ButtonSquare_BeigheDark.png").id,
					Bengine::ColorRGBA8(255, 255, 0, 150));
				// set the continent for the territory
				territory_->setContinent(continents_[5]);
				continents_[5]->setNumberOfTerritories(continents_[5]->getNumberOfTerritories() + 1);
				// place the new territory into all territories vector
				territories_.push_back(territory_);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}
}


Level::~Level()
{
	for (unsigned int i = 0; i < continents_.size(); i++) {
		delete continents_[i];
	}
	for (unsigned int i = 0; i < territories_.size(); i++) {
		delete territories_[i];
	}
}
