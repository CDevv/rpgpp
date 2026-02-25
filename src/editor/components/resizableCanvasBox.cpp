#include "components/resizableCanvasBox.hpp"
#include "raylib.h"

ResizableCanvasBox::ResizableCanvasBox(float x, float y, float width, float height, Color color)
    : x(x), y(y), width(width), height(height), color(color) {}

void ResizableCanvasBox::updateSize(float width, float height) {
    this->width = width;
    this->height = height;
}

void ResizableCanvasBox::updatePosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void ResizableCanvasBox::updateColor(Color color) {
    this->color = color;
}

void ResizableCanvasBox::updateRec(Rectangle rec) {
    this->x = rec.x;
    this->y = rec.y;
    this->width = rec.width;
    this->height = rec.height;
}

void ResizableCanvasBox::leftMousePressed(Vector2 mousePos) {
    // Implement leftMouseReleased logic here
}

void ResizableCanvasBox::mouseMoved(Vector2 mousePos) {
    // Implement mouseMoved logic here
}

void ResizableCanvasBox::leftMouseReleased(Vector2 mousePos) {
    // Implement mousePressed logic here
}

void ResizableCanvasBox::draw() {
	DrawRectangleRec(Rectangle{x, y, width, height}, color);
    // Implement draw logic here
}
