#include "soundService.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include <cstdio>
#include <raylib.h>
#include <stdexcept>

SoundService::SoundService() {
	music = {};
	musicLoaded = false;
	lastId = "";
}

bool SoundService::loadMusic(const std::string &id) {
	GameData &gameData = Game::getBin();

	if (lastId == id)
		return true;

	if (gameData.music.count(id) != 0) {
		if (gameData.music[id].isSound)
			return musicLoaded;

		auto &musicBin = gameData.music[id];

		Music newMusic = LoadMusicStreamFromMemory(musicBin.fileExt.c_str(),
												   musicBin.fileData.data(),
												   musicBin.fileData.size());

		if (IsMusicValid(newMusic)) {
			unload();

			this->music = newMusic;
			musicLoaded = true;
		}
	} else {
		printf("Music with such id does not exist: %s \n", id.c_str());
	}

	return musicLoaded;
}

void SoundService::playMusic() const {
	if (musicLoaded) {
		PlayMusicStream(music);
	}
}

void SoundService::playSound(const std::string &id) const {
	GameData &gameData = Game::getBin();

	if (gameData.music.count(id) != 0) {
		if (gameData.music[id].isSound) {
			MusicBin soundBin = gameData.music[id];
			Wave soundWave = LoadWaveFromMemory(soundBin.fileExt.c_str(),
												soundBin.fileData.data(),
												soundBin.fileData.size());
			Sound sound = LoadSoundFromWave(soundWave);

			PlaySound(sound);

			UnloadWave(soundWave);

			// UnloadSound(sound);
		}
	} else {
		throw std::runtime_error(
			TextFormat("This Sound does not exist: %s", id.c_str()));
	}
}

void SoundService::update() const {
	if (musicLoaded) {
		UpdateMusicStream(music);
	}
}

void SoundService::unload() const {
	if (musicLoaded) {
		StopMusicStream(music);
		UnloadMusicStream(music);
	}
}
