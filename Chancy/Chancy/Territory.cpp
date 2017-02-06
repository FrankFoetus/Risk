#include "Territory.h"



Territory::Territory(std::string name, glm::vec4 position, glm::vec4 uv, GLuint textureID, Bengine::ColorRGBA8 color) :
		name_(name), position_(position), uv_(uv), textureID_(textureID), color_(color)
{
}


Territory::~Territory()
{
}
