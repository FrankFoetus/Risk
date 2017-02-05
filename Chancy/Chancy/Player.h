#pragma once

#include <vector>
#include <string>

#include "Territory.h"

class Player
{
public:
	Player(std::string name);
	~Player();

	std::string getName() { return name_; }
	int getTurnOrder() { return turnOrder_; }
	int getIncome() { return income_; }
	int getEnergy() { return energy_; }
	std::vector<Territory*>& getTerritories() { return territories_; }

	void setName(std::string name) { name_ = name; }

	void setTrunOrder(int turnOrder) { turnOrder_ = turnOrder; }
	void setIncome(int income) { income_ = income; }
	void setEnergy(int energy) { energy_ = energy; }


private:
	std::string name_;

	int turnOrder_;
	int income_ = 3;
	int energy_ = 3;

	std::vector<Territory*> territories_;
};

