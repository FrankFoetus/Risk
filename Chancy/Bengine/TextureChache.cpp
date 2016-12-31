#include "TextureChache.h"
#include "ImageLoader.h"

#include <iostream>


namespace Bengine {

	TextureChache::TextureChache()
	{
	}


	TextureChache::~TextureChache()
	{
	}


	GLTexture TextureChache::getTexture(std::string texturePath) {
		// auto replaces --> std::map<std::string, GLTexture>::iterator 
		// lookup the texture and see if its in the map
		auto mit = _textureMap.find(texturePath);

		// check if its not in the map == nullptr
		if (mit == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// make_pair does this for us -->  std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
			// insert it into the map
			_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "Used Cached Texture!" << std::endl;
			return newTexture;
		}

		std::cout << "Loaded Texture!" << std::endl;
		// mit already points to the texture if it is already in the map
		// so just return it
		return mit->second;
	}

}