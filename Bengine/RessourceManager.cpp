#include "RessourceManager.h"


namespace Bengine {

	TextureChache RessourceManager::_textureCache;


	GLTexture RessourceManager::getTexture(std::string texturePath) {
		return _textureCache.getTexture(texturePath);
	}

}