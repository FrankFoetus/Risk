#include "State.h"



State::State()
{
}


State::~State()
{
}

void State::init(Bengine::InputManager* inputManager, Bengine::Camera2D* camera2D, Bengine::Camera2D* hudCamera2D, Bengine::AudioEngine* audioEngine, Level* level, Bengine::GLSLProgram* colorProgram,
	Bengine::SpriteFont* spriteFont, Bengine::SpriteBatch* territoryBatch, Bengine::SpriteBatch* hudSpriteBatch, Bengine::Window* window, const GLuint windowWidth, const GLuint windowHeight) {
	inputManager_ = inputManager;
	camera2D_ = camera2D;
	hudCamera2D_ = hudCamera2D;
	audioEngine_ = audioEngine;
	level_ = level;
	window_ = window;
	windowWidth_ = windowWidth;
	windowHeight_ = windowHeight;
	colorProgram_ = colorProgram;
	spriteFont_ = spriteFont;
	territoryBatch_ = territoryBatch;
	hudSpritebatch_ = hudSpriteBatch;

	territories_ = level->getTerritories();
	for (auto it : territories_) {
		std::cout << it->getName() << std::endl;
	}
}

void State::drawHud(std::string text, glm::vec2 pos, glm::vec2 size, Bengine::ColorRGBA8 color) {
	// set the camera matrix
	glm::mat4 cameraMatrix = hudCamera2D_->getCameraMatrix();
	GLint pLocation = colorProgram_->getUniformLocation("P");
	// upload matrix to the gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	char buffer[256];

	hudSpritebatch_->begin();

	sprintf_s(buffer, text.c_str());
	spriteFont_->draw(*hudSpritebatch_, buffer, pos, size, 0.0f, color);

	hudSpritebatch_->end();
	hudSpritebatch_->renderBatch();
}



/**
checks if there are any territories hit by mouse click
*/
Territory* State::checkDistanceToTerritory(glm::vec2 mousePos) {
	for (auto it : level_->getTerritories()) {
		glm::vec4 territoryPos = it->getPosition();
		/*
		std::cout << territoryPos.x << " " << territoryPos.y << std::endl;
		std::cout << mousePos.x << " " << mousePos.y << std::endl;
		*/
		if (mousePos.x < territoryPos.x + territoryPos.z && mousePos.x > territoryPos.x && mousePos.y > territoryPos.y && mousePos.y < territoryPos.y + territoryPos.w) {
			return it;
		}
	}
	return nullptr;
}


/**
light up all units and the sprite of territory
*/
void State::lightUpTerritory(Territory* territory) {
	// light up the territory sprite
	territory->setColor(Bengine::ColorRGBA8(territory->getColor().r, territory->getColor().g, territory->getColor().b, 255));
	// light up the sprites of units of type 1
	for (auto it : territory->getUnitsT1()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
	// light up the sprites of units of type 2
	for (auto it : territory->getUnitsT2()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
	// light up the sprites of units of type 3
	for (auto it : territory->getUnitsT3()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 255));
	}
}


/**
light down all units and the sprite of territory
*/
void State::lightDownTerritory(Territory* territory) {
	// dim the territory sprite
	territory->setColor(Bengine::ColorRGBA8(territory->getColor().r, territory->getColor().g, territory->getColor().b, 150));
	// dim the sprites of units of type 1
	for (auto it : territory->getUnitsT1()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
	// dim the sprites of units of type 2
	for (auto it : territory->getUnitsT2()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
	// dim the sprites of units of type 3
	for (auto it : territory->getUnitsT3()) {
		it->setColor(Bengine::ColorRGBA8(it->getColor().r, it->getColor().g, it->getColor().b, 150));
	}
}


/**
performs a unit transformation from tier Tx into tier Ty in territory
*/
void State::unitTransformation(Territory* territory, int Tx, int Ty) {
	std::vector<Unit*> unitsT1 = territory->getUnitsT1();
	std::vector<Unit*> unitsT2 = territory->getUnitsT2();
	std::vector<Unit*> unitsT3 = territory->getUnitsT3();
	switch (Tx) {
	case 1:
		if (Tx < Ty) {
			// delete four T1 units (if they are to be transformed into one T2 unit)
			for (unsigned int i = 0; i < 4; i++) {
				delete unitsT1[i];
			}
			unitsT1.erase(unitsT1.begin(), unitsT1.end());
		}
		else {
			// delete only one T1 unit (if it is to be removed)
			delete unitsT1[unitsT1.size() - 1];
			unitsT1.pop_back();
		}
		break;
	case 2:
		if (Tx < Ty) {
			// delete four T2 and four T1 units (if they are to be transformed into one T3 unit)
			for (unsigned int i = 0; i < 4; i++) {
				delete unitsT2[3 - i];
				delete unitsT1[3 - i];
				unitsT2.pop_back();
				unitsT1.pop_back();
			}
		}
		else {
			// delete only one T2 unit (if it is to be transformed into four T1 units)
			delete unitsT2[unitsT2.size() - 1];
			unitsT2.pop_back();
		}
		break;
	case 3:
		// delete one T3 units (can only go from one T3 unit to four T2 and four T1 units)
		delete unitsT3[unitsT3.size() - 1];
		unitsT3.pop_back();
		break;
	default:
		break;
	}
	switch (Ty) {
	case 1:
		unsigned int j;
		if (Tx < Ty) { j = 1; }  /// if only spawn one T1 unit
		else { j = 4; }  /// if transform a T2 unit into four T1

						 // add j new T1 unit to the unitsT1 vector
		for (unsigned int i = 0; i < j; i++) {
			unitsT1.emplace_back(new Unit(glm::vec4(territory->getPosition().x + (territory->getUnitsT1().size() + i) * territory->getPosition().z / 4,
				territory->getPosition().y, territory->getPosition().z / 4, territory->getPosition().w / 3),
				territory->getUV(),
				Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Mushroom1.png").id,
				Bengine::ColorRGBA8(255, 255, 255, 150),
				audioEngine_->loadSoundEffect("Sounds/slimy_monster1.mp3")));
		}
		break;
	case 2:
		// if four T1 are to be transformed into one T2
		if (Tx < Ty) {
			// add one new T2 unit to the unitsT2 vector
			unitsT2.emplace_back(new Unit(glm::vec4(territory->getPosition().x + territory->getUnitsT2().size() * territory->getPosition().z / 4,
				territory->getPosition().y + territory->getPosition().w / 3, territory->getPosition().z / 4, territory->getPosition().w / 3),
				territory->getUV(),
				Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Broccoli1.png").id,
				Bengine::ColorRGBA8(255, 255, 255, 150),
				audioEngine_->loadSoundEffect("Sounds/slimy_monster2.mp3")));
		}
		// if one T3 unit is to be transformed into four T2 and four T1 units
		else {
			for (unsigned int i = 0; i < 4; i++) {
				// add a new T2 unit to the unitsT2 vector
				unitsT2.emplace_back(new Unit(glm::vec4(territory->getPosition().x + (territory->getUnitsT2().size() + i) * territory->getPosition().z / 4,
					territory->getPosition().y + territory->getPosition().w / 3, territory->getPosition().z / 4, territory->getPosition().w / 3),
					territory->getUV(),
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Broccoli1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 150),
					audioEngine_->loadSoundEffect("Sounds/slimy_monster2.mp3")));
				// add a new T3 unit to the unitsT3 vector
				unitsT1.emplace_back(new Unit(glm::vec4(territory->getPosition().x + (territory->getUnitsT1().size() + i) * territory->getPosition().z / 4,
					territory->getPosition().y, territory->getPosition().z / 4, territory->getPosition().w / 3),
					territory->getUV(),
					Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Mushroom1.png").id,
					Bengine::ColorRGBA8(255, 255, 255, 150),
					audioEngine_->loadSoundEffect("Sounds/slimy_monster1.mp3")));
			}
		}

		break;
	case 3:
		// add one new T3 unit to the unitsT3 vector
		unitsT3.emplace_back(new Unit(glm::vec4(territory->getPosition().x + territory->getUnitsT3().size() * territory->getPosition().z / 4,
			territory->getPosition().y + territory->getPosition().w * 2 / 3, territory->getPosition().z / 4, territory->getPosition().w / 3),
			territory->getUV(),
			Bengine::RessourceManager::getTexture("Textures/jimmyJump_pack/PNG/Enemys/Enemy_Candy1.png").id,
			Bengine::ColorRGBA8(255, 255, 255, 150),
			audioEngine_->loadSoundEffect("Sounds/slimy_monster.mp3")));
		break;
	default:
		break;
	}
	territory->setUnitsT1(unitsT1);
	territory->setUnitsT2(unitsT2);
	territory->setUnitsT3(unitsT3);
}

