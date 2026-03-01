#include "saveables/imageWrapper.hpp"
#include "raylib.h"
#include <cstdio>
#include <string>

ImageWrapper::ImageWrapper(const std::string &filePath) {
	printf("%s \n", filePath.c_str());
	image = LoadImage(filePath.c_str());
}

ImageWrapper::~ImageWrapper() {
	if (IsImageValid(image)) {
		UnloadImage(image);
	}
}