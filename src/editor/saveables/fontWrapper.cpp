#include "saveables/fontWrapper.hpp"
#include "raylib.h"
#include <string>

FontWrapper::FontWrapper(const std::string &filePath) {
	font = LoadFont(filePath.c_str());
}

FontWrapper::~FontWrapper() {
	if (IsFontValid(font)) {
		UnloadFont(font);
	}
}