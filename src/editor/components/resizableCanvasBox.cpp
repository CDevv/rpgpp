#include "components/resizableCanvasBox.hpp"
#include "raylib.h"
#include <cmath>

bool bounded(float value, float min, float max) {
	return value >= min && value <= max;
}

ResizableCanvasBox::ResizableCanvasBox(std::string id, float x, float y,
									   float width, float height, Color color)
	: id(id), x(x), y(y), width(width), height(height), color(color) {}

ResizableCanvasBox::ResizableCanvasBox(std::string id, Rectangle rec,
									   Color color)
	: id(id), x(rec.x), y(rec.y), width(rec.width), height(rec.height),
	  color(color) {}

void ResizableCanvasBox::updateSize(float width, float height) {
	this->width = width;
	this->height = height;
}

void ResizableCanvasBox::updatePosition(float x, float y) {
	this->x = x;
	this->y = y;
}

void ResizableCanvasBox::updateColor(Color color) { this->color = color; }

void ResizableCanvasBox::updateRec(Rectangle rec) {
	this->x = rec.x;
	this->y = rec.y;
	this->width = rec.width;
	this->height = rec.height;
}

bool ResizableCanvasBox::leftMousePressed(Vector2 mousePos) {
	resizeDirection = NONE;
	if (mousePos.x >= x && mousePos.x <= x + width && mousePos.y >= y &&
		mousePos.y <= y + height) {
		if (mousePos.x >= x && mousePos.x <= x + RESIZE_MARGIN) {
			resizeDirection |= LEFT;
		}
		if (mousePos.x >= x + width - RESIZE_MARGIN && mousePos.x <= x + width) {
			resizeDirection |= RIGHT;
		}
		if (mousePos.y >= y && mousePos.y <= y + RESIZE_MARGIN) {
			resizeDirection |= TOP;
		}
		if (mousePos.y >= y + height - RESIZE_MARGIN && mousePos.y <= y + height) {
			resizeDirection |= BOTTOM;
		}
		if (resizeDirection == NONE) {
			resizeDirection |= MOVE;
		}
		isResizing = true;
		startMousePos = mousePos;
		prevX = x;
		prevY = y;
		prevWidth = width;
		prevHeight = height;

		return true;
	}
	return false;
}

void ResizableCanvasBox::mouseMoved(Vector2 mousePos) {
	if (!isResizing || !focused)
		return;

	int dx = std::round(mousePos.x - startMousePos.x);
	int dy = std::round(mousePos.y - startMousePos.y);

	if ((resizeDirection & LEFT) && prevWidth - dx < minSize)
	    dx = prevWidth - minSize;

	if ((resizeDirection & TOP) && prevHeight - dy < minSize)
	    dy = prevHeight - minSize;

	if (resizeDirection & LEFT) {
		x = prevX + dx;
		width = prevWidth - dx;
	}
	if (resizeDirection & RIGHT) {
		width = prevWidth + dx;
	}
	if (resizeDirection & TOP) {
		y = prevY + dy;
		height = prevHeight - dy;
	}
	if (resizeDirection & BOTTOM) {
		height = prevHeight + dy;
	}
	if (resizeDirection & MOVE) {
		x = prevX + dx;
		y = prevY + dy;
	}

	width = std::max(minSize, width);
	height = std::max(minSize, height);
}

Rectangle ResizableCanvasBox::leftMouseReleased(Vector2 mousePos) {
	if (isResizing) {
		isResizing = false;
		resizeDirection = NONE;
	}
	return Rectangle{x, y, width, height};
}

void ResizableCanvasBox::draw() {
	float opacity = 0.2;
	if (focused) {
		opacity = 0.5;
	}
	DrawRectangleLinesEx(Rectangle{x, y, width, height}, 1,
						 Fade(BLACK, opacity));
	DrawRectangleRec(Rectangle{x, y, width, height}, Fade(color, opacity));
	// Implement draw logic here
}
