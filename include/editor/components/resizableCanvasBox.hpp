#ifndef RPGPP_RESIZABLECANVASBOX_H
#define RPGPP_RESIZABLECANVASBOX_H
#include "raylib.h"
#include <string>

enum ResizeDirection { NONE, TOP, BOTTOM, LEFT, RIGHT };

class ResizableCanvasBox {
  public:
	ResizableCanvasBox(std::string id, float x, float y, float width,
					   float height, Color color);
	ResizableCanvasBox(std::string id, Rectangle rec, Color color);
	void draw();

	// @returns true if the box was clicked, false otherwise
	bool leftMousePressed(Vector2 mousePos);
	void mouseMoved(Vector2 mousePos);
	// @returns the new rectangle after the mouse is released
	Rectangle leftMouseReleased(Vector2 mousePos);

	void updatePosition(float x, float y);
	void updateSize(float width, float height);
	void updateColor(Color color);

	void updateRec(Rectangle rec);
	bool focused = false;
	std::string id{};

  private:
	const float RESIZE_MARGIN = 2.f;

	float x, y, width, height;
	Color color = RED;

	bool isResizing = false;
	bool isMoving = false;
	Vector2 startMousePos{};
	float prevX = 0, prevY = 0, prevWidth = 0, prevHeight = 0;
	ResizeDirection resizeDirection = NONE;
};

#endif
