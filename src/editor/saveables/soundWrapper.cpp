#include "saveables/soundWrapper.hpp"
#include "raylib.h"
#include <string>

SoundWrapper::SoundWrapper(const std::string &filePath) {
	sound = LoadMusicStream(filePath.c_str());
}

SoundWrapper::~SoundWrapper() {
	if (IsMusicValid(sound)) {
		UnloadMusicStream(sound);
	}
}