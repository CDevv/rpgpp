#ifndef RPGPP_RESIZABLECANVASBOX_H
#define RPGPP_RESIZABLECANVASBOX_H
#include "raylib.h"
#include <string>

enum ResizeDirection {
	NONE = 0, MOVE = 1 << 0, TOP = 1 << 1, BOTTOM = 1 << 2, LEFT = 1 << 3, RIGHT = 1 << 4 };

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

	void setMinSize(float size) { minSize = std::max(size, 2.f); }
	float getMinSize() const { return minSize; }
  private:
	const float RESIZE_MARGIN = 1.f;
	float minSize = 2.f;

	float x, y, width, height;
	Color color = RED;

	bool isResizing = false;
	Vector2 startMousePos{};
	float prevX = 0, prevY = 0, prevWidth = 0, prevHeight = 0;
	int resizeDirection = NONE;
};

#endif
