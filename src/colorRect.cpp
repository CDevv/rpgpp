#include "colorRect.hpp"
#include <raylib.h>

ColorRect::ColorRect() : rect(Rectangle{1, 1, 1, 1}), color() {}

ColorRect::ColorRect(const Rectangle rect) {
	this->rect = rect;
	this->color = RAYWHITE;
}

void ColorRect::setColor(const Color newColor) { this->color = newColor; }

void ColorRect::update() {
	// TODO
}

void ColorRect::draw() { DrawRectangleRec(rect, color); }
