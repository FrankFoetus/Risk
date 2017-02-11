#include "AudioEngine.h"
#include "BengineError.h"

namespace Bengine {


	void SoundEffect::play(int loops /* = 0 */)
	{
		if (Mix_PlayChannel(-1, _chunk, loops) == -1) {
			if (Mix_PlayChannel(0, _chunk, loops) == -1) {
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}



	void Music::play(int loops /* = -1 */)
	{
		Mix_PlayMusic(_music, loops);
	}


	void Music::pause() {
		Mix_PauseMusic();
	}


	void Music::stop() {
		Mix_HaltMusic();
	}


	void Music::resume() {
		Mix_ResumeMusic();
	}


	AudioEngine::AudioEngine()
	{
	}


	AudioEngine::~AudioEngine()
	{
		destroy();
	}


	void AudioEngine::init()
	{
		// Parameter can be a bitwise combination of MIX_INIT_FAC,
		// MIX_INIT, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 1) {
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == 1) {
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}
		_isInitialized = true;
	}


	void AudioEngine::destroy()
	{
		if (_isInitialized) {
			_isInitialized = false;
			Mix_Quit();
		}
	}


	SoundEffect AudioEngine::loadSoundEffect(const std::string & filePath)
	{
		// try to find the audio in the cache
		auto it = _effectMap.find(filePath);
		
		SoundEffect effect;

		if (it == _effectMap.end()) {
			// failed to find it, must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			effect._chunk = chunk;
			_effectMap[filePath] = chunk;

		} else {
			// its allready cached
			effect._chunk = it->second;
		}

		return effect;
	}


	Music AudioEngine::loadMusic(const std::string & filePath)
	{
		// try to find the audio in the cache
		auto it = _musicMap.find(filePath);

		Music music;

		if (it == _musicMap.end()) {
			// failed to find it, must load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			music._music = mixMusic;
			_musicMap[filePath] = mixMusic;

		}
		else {
			// its allready cached
			music._music = it->second;
		}

		return music;
	}

}