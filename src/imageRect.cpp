#include "imageRect.hpp"

#include <raylib.h>

#include <game.hpp>

ImageRect::ImageRect() : rect(Rectangle{}), texture() {}

ImageRect::ImageRect(Rectangle rect) : texture() {
	this->rect = rect;
	this->source = "";

	props->push_back({"rect_x", rect.x});
	props->push_back({"rect_y", rect.y});
	props->push_back({"rect_width", rect.width});
	props->push_back({"rect_height", rect.height});

	props->push_back({"source", source});
}

void ImageRect::setSource(const std::string &source) { this->source = source; }

void ImageRect::setTexture(Texture2D texture) { this->texture = texture; }

void ImageRect::update() {}

void ImageRect::draw() {
	Rectangle rect = {props->at("rect_x"), props->at("rect_y"), props->at("rect_width"), props->at("rect_height")};
	DrawTexturePro(texture, Rectangle{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
				   rect, Vector2{0, 0}, 0.0f, WHITE);
}
