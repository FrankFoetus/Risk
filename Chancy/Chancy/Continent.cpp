#include "Continent.h"


Continent::Continent(std::string name, int bonus) :
		name_(name), bonus_(bonus)
{
}

Continent::~Continent()
{
}

bool Continent::checkCompletion(Player* player, std::vector<Territory*> territories) {
	int countedTerritories = 0;
	for (auto territory : territories) {
		if (territory->getOwner() == player && territory->getContinent() == this) {
			countedTerritories++;
		}
	}
	if (countedTerritories == numberOfTerritories_) {
		return true;
	}
	return false;
}