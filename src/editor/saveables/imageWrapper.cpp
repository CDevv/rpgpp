#include "saveables/imageWrapper.hpp"

#include <cstdio>
#include <string>

#include "raylib.h"

ImageWrapper::ImageWrapper(const std::string &filePath) {
	printf("%s \n", filePath.c_str());
	image = LoadImage(filePath.c_str());
	source = filePath;
}

ImageWrapper::~ImageWrapper() {
	if (IsImageValid(image)) {
		UnloadImage(image);
	}
}