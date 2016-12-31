#include "InputManager.h"

namespace Bengine {

	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update() {
		// loop through _keyMap with "for each"
		// for (auto it = _keyMap.begin(); it != _keyMap.end(); it++) {
		for(auto& it : _keyMap){
			_previousKeyMap[it.first] = it.second;
		}
	}


	void InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyDown(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		}
		return false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		// check if key is pressed this frame and wasnt pressed last frame
		if (isKeyDown(keyID) && !wasKeyDown(keyID)) {
			return true;
		}
		return false;
	}

	bool InputManager::wasKeyDown(unsigned int keyID) {
		auto it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end()) {
			return it->second;
		}
		return false;
	}
}
