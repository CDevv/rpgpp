#ifndef _RPGPP_EDITOR_MOUSEINPUT_H
#define _RPGPP_EDITOR_MOUSEINPUT_H

#include <raylib.h>

class MouseInputComponent {
private:
    Camera2D& camera;
    Rectangle cameraRect;
    Vector2 offset;
    Vector2 mousePos;
    Vector2 hoverPos;
    Vector2 lastPos;
    bool lock;
    int lastMode;
public:
    MouseInputComponent(Vector2 offset, Camera2D& camera, Rectangle cameraRect);
    Vector2 getMouseWorldPos();
    Vector2 getMousePos();
    void update();
    bool isInRect();
};

#endif
