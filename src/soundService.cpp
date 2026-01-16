#include "soundService.hpp"
#include <raylib.h>
#include "game.hpp"

SoundService::SoundService()
{
    music = {};
    musicLoaded = false;
    lastId = "";
}

bool SoundService::loadMusic(const std::string &id)
{
    GameData& gameData = Game::getBin();

    if (lastId == id) return true;

    if (gameData.music.count(id) != 0) {
        if (gameData.music[id].isSound) return musicLoaded;

        Music newMusic = LoadMusicStream(gameData.music[id].relativePath.c_str());
        if (IsMusicValid(newMusic)) {
            unload();

            this->music = newMusic;
            musicLoaded = true;
        }
    }

    return musicLoaded;
}

void SoundService::playMusic() const
{
    if (musicLoaded) {
        PlayMusicStream(music);
    }
}

void SoundService::playSound(const std::string &id) const
{
    GameData& gameData = Game::getBin();

    if (gameData.music.count(id) != 0) {
        if (gameData.music[id].isSound) {
            Sound sound = LoadSound(gameData.music[id].relativePath.c_str());

            PlaySound(sound);
        }
    }
}

void SoundService::update() const
{
    if (musicLoaded) {
        UpdateMusicStream(music);
    }
}

void SoundService::unload() const
{
    if (musicLoaded) {
        StopMusicStream(music);
        UnloadMusicStream(music);
    }
}
