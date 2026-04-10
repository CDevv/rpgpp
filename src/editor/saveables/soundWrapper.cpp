#include "saveables/soundWrapper.hpp"

#include <string>

#include "raylib.h"

SoundWrapper::SoundWrapper(const std::string &filePath) {
	sound = LoadMusicStream(filePath.c_str());
	source = filePath;
}

SoundWrapper::~SoundWrapper() {
	if (IsMusicValid(sound)) {
		UnloadMusicStream(sound);
	}
}