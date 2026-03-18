#include "soundService.hpp"
#include "game.hpp"
#include <cstdio>
#include <raylib.h>

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

		printf("loaded.. \n");

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
			Sound sound = LoadSound(gameData.music[id].relativePath.c_str());

			PlaySound(sound);
		}
	}
}

void SoundService::update() const {
	if (musicLoaded) {
		UpdateMusicStream(music);
	}
}

void SoundService::unload() const {
	if (musicLoaded) {
		printf("unloaded.. \n");
		StopMusicStream(music);
		UnloadMusicStream(music);
	}
}
