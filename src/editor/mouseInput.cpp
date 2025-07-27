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
    mouseWorldPos = Vector2 { 0, 0 };
    cameraTargetOffset = Vector2 { 0, 0 };
    lock = false;
}

void MouseInputComponent::update()
{
    Vector2 mouseDelta = GetMouseDelta();
    Vector2 cameraScale = Vector2Scale(mouseDelta, 1/camera.zoom);

    Vector2 screenMousePos = GetMousePosition();
    mousePos = Vector2Subtract(screenMousePos, offset);
    if (mouseWorldPos.x == 0 && mouseWorldPos.y == 0) {
        mouseWorldPos = mousePos;
    }
    if (!lock) {
        mouseWorldPos = Vector2Add(mouseWorldPos, cameraScale);
    }

    int actionMode = -1;
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        actionMode = 0;
    } else if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
        actionMode = 1;
    }

    EditorInterfaceService& ui = Editor::getUi();
    if (!isInRect() || ui.getMouseLock()) {
        return;
    }

    if (actionMode == 0) {
        //mouse wheel
        Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, camera);
        camera.offset = mousePos;
        camera.target = mouseWorldPos;

        float scale = 0.2f * wheel;
        camera.zoom = Clamp(camera.zoom + scale, 1.0f, 5.0f);
    } else if (actionMode == 1) {
        //mouse hold and move
        lock = true;
        Vector2 cameraMoveScale = Vector2Scale(mouseDelta, -1/camera.zoom);
        camera.target = Vector2Add(camera.target, cameraMoveScale);
        cameraTargetOffset = Vector2Add(cameraTargetOffset, cameraMoveScale);
    }

    if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)) {
        lock = false;
        Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, camera);
        this->mouseWorldPos = mouseWorldPos;
    }

    hoverPos = mousePos;
}

Vector2 MouseInputComponent::getMouseWorldPos()
{
    return mouseWorldPos;
}

Vector2 MouseInputComponent::getMousePos()
{
    return mousePos;
}

bool MouseInputComponent::isInRect()
{
    return CheckCollisionPointRec(GetMousePosition(), cameraRect);
}
