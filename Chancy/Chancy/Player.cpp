#include "Player.h"



Player::Player(std::string name, Bengine::ColorRGBA8 color) : name_(name), color_(color)
{
	// empty constructor
}


Player::Player()
{
	name_ = "Ms. Who";
}


Player::~Player()
{
}
