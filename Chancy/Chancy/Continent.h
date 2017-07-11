#pragma once

#include <string>
#include <vector>

#include "Player.h"
#include "Territory.h"

class Player;
class Territory;

class Continent
{
public:
	Continent(std::string name, int bonus);
	~Continent();

	// getters
	std::string getName() { return name_; }
	Player* getOwner() { return owner_; }
	int getBonus() { return bonus_; }
	int getNumberOfTerritories() { return numberOfTerritories_; }

	// setters
	void setName(std::string name) { name_ = name; }
	void setOwner(Player* owner) { owner_ = owner; }
	void setNumberOfTerritories(int number) { numberOfTerritories_ = number; }

	bool checkCompletion(Player* player, std::vector<Territory*> territories);

private:
	std::string name_;
	Player* owner_;

	int numberOfTerritories_ = 0;
	int bonus_ = 0;
};

