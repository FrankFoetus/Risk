#pragma once

#include <vector>
#include <string>

#include <Bengine\Vertex.h>

#include "Territory.h"

class Territory;
class Continent;

class Player
{
public:
	Player();
	~Player();

	std::string getName() { return name_; }
	int getTurnOrder() { return turnOrder_; }
	int getEnergy() { return energy_; }
	Bengine::ColorRGBA8 getColor() { return color_; }

	void setName(std::string name) { name_ = name; }
	void setTrunOrder(int turnOrder) { turnOrder_ = turnOrder; }
	void setEnergy(int energy) { energy_ = energy; }
	void setColor(Bengine::ColorRGBA8 color) { color_ = color; }


private:
	std::string name_;
	Bengine::ColorRGBA8 color_;
	int turnOrder_;
	int energy_ = 3;
};

