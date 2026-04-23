#include "colorRect.hpp"

#include <raylib.h>

ColorRect::ColorRect() : ColorRect({1, 1, 1, 1}) {}

ColorRect::ColorRect(const Rectangle rect) {
	this->rect = rect;
	this->color = RAYWHITE;

	props->push_back({"rect_x", rect.x});
	props->push_back({"rect_y", rect.y});
	props->push_back({"rect_width", rect.width});
	props->push_back({"rect_height", rect.height});

	props->at("color_r") = color.r;
	props->at("color_g") = color.g;
	props->at("color_b") = color.b;
	props->at("color_a") = color.a;
}

void ColorRect::setColor(const Color newColor) {
	this->color = newColor;
	props->at("color_r") = newColor.r;
	props->at("color_g") = newColor.g;
	props->at("color_b") = newColor.b;
	props->at("color_a") = newColor.a;
}

void ColorRect::update() {
	// TODO
}

void ColorRect::draw() {
	Rectangle rect = {props->at("rect_x"), props->at("rect_y"), props->at("rect_width"), props->at("rect_height")};
	Color color = {props->at("color_r"), props->at("color_g"), props->at("color_b")};
	DrawRectangleRec(rect, color);
}
