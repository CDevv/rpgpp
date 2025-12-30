#include "soundService.hpp"
#include <raylib.h>
#include "game.hpp"

SoundService::SoundService()
{
    musicLoaded = false;
    lastId = "";
}

bool SoundService::loadMusic(std::string id)
{
    GameData& gameData = Game::getBin();

    if (lastId == id) return true;

    if (gameData.music.count(id) != 0) {
        if (gameData.music[id].isSound) return musicLoaded;

        Music music = LoadMusicStream(gameData.music[id].relativePath.c_str());
        if (IsMusicValid(music)) {
            unload();

            this->music = music;
            musicLoaded = true;
        }
    }

    return musicLoaded;
}

void SoundService::playMusic()
{
    if (musicLoaded) {
        PlayMusicStream(music);
    }
}

void SoundService::playSound(std::string id)
{
    GameData& gameData = Game::getBin();

    if (gameData.music.count(id) != 0) {
        if (gameData.music[id].isSound) {
            Sound sound = LoadSound(gameData.music[id].relativePath.c_str());

            PlaySound(sound);
        }
    }
}

void SoundService::update()
{
    if (musicLoaded) {
        UpdateMusicStream(music);
    }
}

void SoundService::unload()
{
    if (musicLoaded) {
        StopMusicStream(music);
        UnloadMusicStream(music);
    }
}
