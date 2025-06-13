#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <memory>
#include <raylib.h>
#include <string>
#include "tileset.hpp"
#include "mouseInput.hpp"
#include "worldViewBox.hpp"

class MouseInputComponent;

class EditorInterfaceService {
private:
    Font uiFont;
    std::unique_ptr<MouseInputComponent> mouseInput;
    WorldViewBox worldView;
    Vector2 mousePos;
    Vector2 hoverPos;
    Camera2D camera;
public:
    EditorInterfaceService();
    void update();
    void draw();
    void unload();
    MouseInputComponent& getMouse();
    Font getFont();
    Camera2D& getCamera();
};

#endif
