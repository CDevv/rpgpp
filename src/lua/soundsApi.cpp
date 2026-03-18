#include "lua/soundsApi.hpp"
#include "game.hpp"
#include "soundService.hpp"
#include <cstdio>

void lua_sounds_loadMusic(const std::string &id) {
	Game::getSounds().loadMusic(id);
}

void lua_sounds_playMusic() { Game::getSounds().playMusic(); }

void lua_sounds_playSound(const std::string &id) {
	Game::getSounds().playSound(id);
}

void lua_sounds_set(sol::state_view &lua) {
	printf("setting sounds api.. \n");
	auto space = lua["Sounds"].get_or_create<sol::table>();
	space.set_function("PlaySound", &SoundService::playSound,
					   Game::getSounds());
	space.set_function("LoadMusic", lua_sounds_loadMusic);
	space.set_function("PlayMusic", lua_sounds_playMusic);
}