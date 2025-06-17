#ifndef _RPGPP_EDITOR_WORLDVIEWBOX_H
#define _RPGPP_EDITOR_WORLDVIEWBOX_H

#include <memory>
#include <raylib.h>
#include "mouseInput.hpp"
#include "tileset.hpp"

class WorldViewBox {
private:
    Rectangle windowRect;
    Rectangle renderRect;
    Camera2D camera;
    std::unique_ptr<MouseInputComponent> mouseInput;
    TileSet *tileSet;
    RenderTexture renderTexture;
    std::string windowTitle;
    Vector2 mousePos;
    Vector2 hoverPos;
public:
    WorldViewBox();
    WorldViewBox(Rectangle windowRect, Rectangle renderRect);
    void update();
    void draw();
    void unload();
};

#endif
