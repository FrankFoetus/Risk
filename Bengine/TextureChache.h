#pragma once
#include <map>

#include "GLTexture.h"

namespace Bengine {

	class TextureChache
	{
	public:
		TextureChache();
		~TextureChache();

		GLTexture getTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> _textureMap;
	};

}