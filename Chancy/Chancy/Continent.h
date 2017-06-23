#pragma once

#include <string>
#include <vector>

#include "Player.h"

class Player;

class Continent
{
public:
	Continent(std::string name);
	~Continent();

	std::string getName() { return name_; }
	Player* getOwner() { return owner_; }
	int getBonus() { return bonus_; }

	void setName(std::string name) { name_ = name; }
	void setOwner(Player* owner) { owner_ = owner; }

private:
	std::string name_;
	Player* owner_;

	int bonus_;
};

