#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <raylib.h>
#include <string>
#include "tileset.hpp"
#include "mouseInput.hpp"
#include "worldViewBox.hpp"

class MouseInputComponent;

class EditorInterfaceService {
private:
    Font uiFont;
    MouseInputComponent *mouseInput;
    WorldViewBox worldView;
    bool openDialog;
    char *tilePath;
    bool openedTileSet;
    Vector2 mousePos;
    Vector2 hoverPos;
    TileSet *tileSet;
    Camera2D camera;
    RenderTexture renderTexture;
public:
    EditorInterfaceService();
    void update();
    void draw();
    void unload();
    MouseInputComponent *getMouse();
    Font getFont();
    Camera2D *getCamera();
    bool hasOpenedTileSet();
    TileSet *getTileSet();
    std::string getTilePath();
};

#endif
