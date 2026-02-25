#ifndef RPGPP_RESIZABLECANVASBOX_H
#define RPGPP_RESIZABLECANVASBOX_H
#include "raylib.h"

class ResizableCanvasBox {
public:
    ResizableCanvasBox(float x, float y, float width, float height, Color color);
    void draw();

    void leftMousePressed(Vector2 mousePos);
    void mouseMoved(Vector2 mousePos);
    void leftMouseReleased(Vector2 mousePos);
    void updatePosition(float x, float y);
    void updateSize(float width, float height);
    void updateColor(Color color);

    void updateRec(Rectangle rec);
private:
    float x, y, width, height;
    Color color = RED;

    bool focused = false;
    bool isResizing = false;
};

#endif
