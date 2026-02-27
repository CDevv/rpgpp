#include "components/resizableCanvasBox.hpp"
#include "raylib.h"
#include <cmath>

bool bounded(float value, float min, float max) {
    return value >= min && value <= max;
}

ResizableCanvasBox::ResizableCanvasBox(std::string id, float x, float y, float width, float height, Color color)
    : id(id), x(x), y(y), width(width), height(height), color(color) {}

ResizableCanvasBox::ResizableCanvasBox(std::string id, Rectangle rec, Color color)
    : id(id), x(rec.x), y(rec.y), width(rec.width), height(rec.height), color(color) {}

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

bool ResizableCanvasBox::leftMousePressed(Vector2 mousePos) {
	if (mousePos.x >= x && mousePos.x <= x + width && mousePos.y >= y && mousePos.y <= y + height) {
        if (mousePos.x >= x && mousePos.x <= x + RESIZE_MARGIN) {
            resizeDirection = LEFT;
        } else if (mousePos.x >= x + width - RESIZE_MARGIN && mousePos.x <= x + width) {
            resizeDirection = RIGHT;
        } else if (mousePos.y >= y && mousePos.y <= y + RESIZE_MARGIN) {
            resizeDirection = TOP;
        } else if (mousePos.y >= y + height - RESIZE_MARGIN && mousePos.y <= y + height) {
            resizeDirection = BOTTOM;
        } else {
            resizeDirection = NONE;
            isMoving = true;
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
	// switch (resizeDirection) {
	// 	case ResizeDirection::LEFT:
	// 	case ResizeDirection::RIGHT:
	// 		SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
	// 		break;
	// 	case ResizeDirection::TOP:
	// 	case ResizeDirection::BOTTOM:
	// 		SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
	// 		break;
	// 	case ResizeDirection::NONE:
	// 		if (isMoving) {
	// 			SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
	// 		} else {
	// 			SetMouseCursor(MOUSE_CURSOR_ARROW);
	// 		}
	// 		break;
	// }

	if (!isResizing || !focused)
		return;
	switch (resizeDirection) {
		case ResizeDirection::LEFT:
			x = prevX + std::round(mousePos.x - startMousePos.x);
			width = prevWidth - std::round(mousePos.x - startMousePos.x);
			break;
		case ResizeDirection::RIGHT:
			width = prevWidth + std::round(mousePos.x - startMousePos.x);
			break;
		case ResizeDirection::TOP:
			y = prevY + std::round(mousePos.y - startMousePos.y);
			height = prevHeight - std::round(mousePos.y - startMousePos.y);
			break;
		case ResizeDirection::BOTTOM:
			height = prevHeight + std::round(mousePos.y - startMousePos.y);
			break;
		case ResizeDirection::NONE:
			if (!isMoving)
				break;
			x = prevX + std::round(mousePos.x - startMousePos.x);
			y = prevY + std::round(mousePos.y - startMousePos.y);
			break;
		default:
			break;
	}

	width = std::max(2.f, width);
	height = std::max(2.f, height);
}

Rectangle ResizableCanvasBox::leftMouseReleased(Vector2 mousePos) {
	if (isResizing) {
        isResizing = false;
        isMoving = false;
        resizeDirection = NONE;
    }
	return Rectangle{x, y, width, height};
}

void ResizableCanvasBox::draw() {
	float opacity = 0.2;
	if (focused) {
		opacity = 0.5;
	}
	DrawRectangleLinesEx(Rectangle{x, y, width, height}, 1, Fade(BLACK, opacity));
	DrawRectangleRec(Rectangle{x, y, width, height}, Fade(color, opacity));
    // Implement draw logic here
}
