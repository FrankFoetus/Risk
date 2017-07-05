#include "Territory.h"



Territory::Territory(std::string name, glm::vec4 position, glm::vec4 uv, GLuint textureID, Bengine::ColorRGBA8 color) :
		name_(name), position_(position), uv_(uv), textureID_(textureID), color_(color)
{
	origColor_ = color;
}


Territory::~Territory()
{
}


void Territory::draw(Bengine::SpriteBatch* spriteBatch) {
	spriteBatch->draw(position_, uv_, textureID_, 0, color_);
	drawUnits(spriteBatch);
}


void Territory::drawUnits(Bengine::SpriteBatch* spriteBatch) {
	for (auto it : unitsT1_) {
		it->draw(spriteBatch);
	}
	for (auto it : unitsT2_) {
		it->draw(spriteBatch);
	}
	for (auto it : unitsT3_) {
		it->draw(spriteBatch);
	}
}


void Territory::addUnit(Bengine::AudioEngine* audioEngine) {
	// if there are already four T1 units, delete them and spawn one T2 unit
	if (unitsT1_.size() == 4) {
		// if there are already four T2 units, delete them and spawn one T3 unit
		if (unitsT2_.size() == 4) {
			unitTransformation(audioEngine, 2, 3);
			unitsT3_[unitsT3_.size() - 1]->getSpawnSoundEffect().play();
		}
		else {
			unitTransformation(audioEngine, 1, 2);
			unitsT2_[unitsT2_.size() - 1]->getSpawnSoundEffect().play();
		}
	}
	else {
		// spawn one T1
		unitTransformation(audioEngine, 0, 1);
		unitsT1_[unitsT1_.size() - 1]->getSpawnSoundEffect().play();
	}
}


void Territory::destroyUnit(Bengine::AudioEngine* audioEngine) {
	// if there are no more T1 units, delete one T2 unit (if available) and spawn four T1 units
	if (unitsT1_.size() == 0) {
		if (unitsT2_.size() == 0) {
			// if there are no more T2 units, delete one T3 unit (if available) and spawn four T1 units
			if (unitsT3_.size() != 0) {
				unitTransformation(audioEngine, 3, 2);
			}
			else {
				std::cout << "NO MORE UNITS HERE!!" << std::endl;
			}
		}
		else {
			unitTransformation(audioEngine, 2, 1);
		}
	}
	else {
		unitTransformation(audioEngine, 1, 0);
	}
}


/**
light up all units and the sprite of territory
*/
void Territory::lightUpTerritory() {
	// light up the territory sprite
	setColor(Bengine::ColorRGBA8(color_.r, color_.g, color_.b, 255));
	// light up the sprites of units of type 1
	for (auto it : unitsT1_) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
	// light up the sprites of units of type 2
	for (auto it : unitsT2_) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
	// light up the sprites of units of type 3
	for (auto it : unitsT3_) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
}


/**
light down all units and the sprite of territory
*/
void Territory::lightDownTerritory() {
	// dim the territory sprite
	setColor(Bengine::ColorRGBA8(color_.r, color_.g, color_.b, 150));
	// dim the sprites of units of type 1
	for (auto it : unitsT1_) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
	// dim the sprites of units of type 2
	for (auto it : unitsT2_) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
	// dim the sprites of units of type 3
	for (auto it : unitsT3_) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
}


/**
performs a unit transformation from tier Tx into tier Ty in territory
*/
void Territory::unitTransformation(Bengine::AudioEngine* audioEngine, int Tx, int Ty) {
	switch (Tx) {
	case 1:
		if (Tx < Ty) {
			// delete four T1 units (if they are to be transformed into one T2 unit)
			for (unsigned int i = 0; i < 4; i++) {
				delete unitsT1_[i];
			}
			unitsT1_.erase(unitsT1_.begin(), unitsT1_.end());
		}
		else {
			// delete only one T1 unit (if it is to be removed)
			delete unitsT1_[unitsT1_.size() - 1];
			unitsT1_.pop_back();
		}
		break;
	case 2:
		if (Tx < Ty) {
			// delete four T2 and four T1 units (if they are to be transformed into one T3 unit)
			for (unsigned int i = 0; i < 4; i++) {
				delete unitsT2_[3 - i];
				delete unitsT1_[3 - i];
				unitsT2_.pop_back();
				unitsT1_.pop_back();
			}
		}
		else {
			// delete only one T2 unit (if it is to be transformed into four T1 units)
			delete unitsT2_[unitsT2_.size() - 1];
			unitsT2_.pop_back();
		}
		break;
	case 3:
		// delete one T3 units (can only go from one T3 unit to four T2 and four T1 units)
		delete unitsT3_[unitsT3_.size() - 1];
		unitsT3_.pop_back();
		break;
	default:
		break;
	}
	switch (Ty) {
	case 1:
		unsigned int j;
		if (Tx < Ty) { j = 1; }  /// if only spawn one T1 unit
		else { j = 4; }  /// if transform a T2 unit into four T1

		// add j new T1 units to the unitsT1 vector
		for (unsigned int i = 0; i < j; i++) {
			unitsT1_.emplace_back(new Unit(glm::vec4(position_.x + unitsT1_.size() * position_.z / 4,
				position_.y, position_.z / 4, position_.w / 3),
				uv_,
				Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Mushroom1.png").id,
				Bengine::ColorRGBA8(255, 255, 255, 255),
				audioEngine->loadSoundEffect("Sounds/slimy_monster1.mp3")));
		}
		break;
	case 2:
		// if four T1 are to be transformed into one T2
		if (Tx < Ty) {
			// add one new T2 unit to the unitsT2 vector
			unitsT2_.emplace_back(new Unit(glm::vec4(position_.x + unitsT2_.size() * position_.z / 4,
				position_.y + position_.w / 3, position_.z / 4, position_.w / 3),
				uv_,
				Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Broccoli1.png").id,
				Bengine::ColorRGBA8(255, 255, 255, 255),
				audioEngine->loadSoundEffect("Sounds/slimy_monster2.mp3")));
		}
		// if one T3 unit is to be transformed into four T2 and four T1 units
		else {
			for (unsigned int i = 0; i < 4; i++) {
				// add a new T2 unit to the unitsT2 vector
				unitsT2_.emplace_back(new Unit(glm::vec4(position_.x + unitsT2_.size() * position_.z / 4,
					position_.y + position_.w / 3, position_.z / 4, position_.w / 3),
					uv_,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Broccoli1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 255),
					audioEngine->loadSoundEffect("Sounds/slimy_monster2.mp3")));
				// add a new T3 unit to the unitsT3 vector
				unitsT1_.emplace_back(new Unit(glm::vec4(position_.x + unitsT1_.size() * position_.z / 4,
					position_.y, position_.z / 4, position_.w / 3),
					uv_,
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Mushroom1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 255),
					audioEngine->loadSoundEffect("Sounds/slimy_monster1.mp3")));
			}
		}

		break;
	case 3:
		// add one new T3 unit to the unitsT3 vector
		unitsT3_.emplace_back(new Unit(glm::vec4(position_.x + unitsT3_.size() * position_.z / 4,
			position_.y + position_.w * 2 / 3, position_.z / 4, position_.w / 3),
			uv_,
			Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Candy1.png").id,
			Bengine::ColorRGBA8(255, 255, 255, 255),
			audioEngine->loadSoundEffect("Sounds/slimy_monster.mp3")));
		break;
	default:
		break;
	}
}


void Territory::update() {
}
