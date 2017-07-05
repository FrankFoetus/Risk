#pragma once

#include <vector>
#include <time.h>
#include <GL\glew.h>

#include <Bengine\RessourceManager.h>
#include <Bengine\SpriteBatch.h>
#include <Bengine\Vertex.h>

#include "Util.h"

class Dice
{
public:
	Dice();
	Dice(int numberOfDice, glm::vec2 upperLeftCornerPos, GLuint dieSize);
	~Dice();

	void draw(Bengine::SpriteBatch* spriteBatch);
	void roll(int numberOfDice);

	// getters
	std::vector<int> getRoll() { return currentRoll_; }
	std::vector<bool> getActivation() { return active_; }
	GLuint getDieSize() { return dieSize_; }

	// setters
	void setActivation(bool active, int ind) { 
		if (ind < active_.size()) { active_[ind] = active; } 
	}
	void setDieSize(GLuint size) { dieSize_ = size; }

private:
	GLuint dieSize_ = 50;

	std::vector<bool> active_;
	std::vector<int> currentRoll_;
	std::vector<int> textureIDs_;
	std::vector<glm::vec4> position_;
	std::vector<Bengine::ColorRGBA8> color_;
};

