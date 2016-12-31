#pragma once
#include "TextureChache.h"

#include <string>

namespace Bengine {

	class RessourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureChache _textureCache;
	};

}
