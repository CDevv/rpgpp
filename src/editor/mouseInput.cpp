#include "mouseInput.hpp"
#include <raymath.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"

MouseInputComponent::MouseInputComponent(Vector2 offset, Camera2D& camera, Rectangle cameraRect)
: camera(camera)
{
    this->offset = offset;
    this->camera = camera;
    this->cameraRect = cameraRect;
    this->lastMode = 0;

    mousePos = Vector2 { 0, 0 };
    hoverPos = Vector2 { 0, 0 };
    lastPos = Vector2Subtract(hoverPos, offset);
    lock = false;
}

void MouseInputComponent::update()
{
    Vector2 screenMousePos = GetMousePosition();
    mousePos = Vector2Subtract(screenMousePos, offset);

    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f/camera.zoom);

    if (!lock) {
        lastPos = Vector2Add(lastPos, Vector2Scale(GetMouseDelta(), 1/camera.zoom));
    }

    hoverPos = lastPos;

    EditorInterfaceService& ui = Editor::getUi();
    if (!isInRect() || ui.getMouseLock()) {
        return;
    }

    if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
        camera.target = Vector2Add(camera.target, delta);

        lastPos = hoverPos;
        lock = true;
        lastMode = 1;
    } else {
        lock = false;
    }

    if (lastMode == 0) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, camera);
            camera.offset = mousePos;
            camera.target = mouseWorldPos;

            float scale = 0.2f * wheel;
            camera.zoom = Clamp(camera.zoom + scale, 1.0f, 5.0f);
        }
    }

    lastMode = 0;
}

Vector2 MouseInputComponent::getMouseWorldPos()
{
    return hoverPos;
}

Vector2 MouseInputComponent::getMousePos()
{
    return mousePos;
}

bool MouseInputComponent::isInRect()
{
    return CheckCollisionPointRec(GetMousePosition(), cameraRect);
}
