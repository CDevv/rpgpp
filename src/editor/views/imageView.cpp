#include "views/imageView.hpp"

#include "raylib.h"
#include "saveables/imageWrapper.hpp"

ImageView::ImageView() {}

ImageView::~ImageView() {
	if (IsTextureValid(texture)) {
		UnloadTexture(texture);
	}
}

ImageView::Ptr ImageView::create() { return std::make_shared<ImageView>(); }

void ImageView::setImage(ImageWrapper *image) {
	this->image = image;
	texture = LoadTextureFromImage(image->image);
}

void ImageView::drawCanvas() {
	ClearBackground(RAYWHITE);

	if (image == nullptr) {
		return;
	}

	if (!IsTextureValid(texture)) {
		return;
	}

	drawOrigin();

	DrawTexture(texture, 0, 0, WHITE);
}

void ImageView::drawOverlay() {}