#include "saveables/fontWrapper.hpp"

#include <string>

#include "raylib.h"

FontWrapper::FontWrapper(const std::string &filePath) {
	font = LoadFont(filePath.c_str());
	fontSource = filePath;
}

FontWrapper::~FontWrapper() {
	if (IsFontValid(font)) {
		UnloadFont(font);
	}
}