#pragma once

#include <string>

class Card
{
public:
	Card();
	~Card();

	std::string getName() { return name_; };
	std::string getDescription() { return description_; };

	void setName(std::string name) { name_ = name; };
	void setDescription(std::string description) { description_ = description; };

	void effect();

private:
	std::string name_;
	std::string description_;
};

