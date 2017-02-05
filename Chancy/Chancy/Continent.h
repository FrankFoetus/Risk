#pragma once

#include <string>
#include <vector>

class Territory;

class Continent
{
public:
	Continent(std::string name);
	~Continent();

	std::string getName() { return name_; }
	std::vector<Territory*>& getTerritories() { return territories_; }

	void setName(std::string name) { name_ = name; }

private:
	std::string name_;

	int bonus_;

	std::vector<Territory*> territories_;
};

