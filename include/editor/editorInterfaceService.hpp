#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <raylib.h>
#include <string>
#include "tileset.hpp"

class EditorInterfaceService {
private:
    Font uiFont;
    bool openDialog;
    char *tilePath;
    bool openedTileSet;
    Vector2 mousePos;
    Vector2 hoverPos;
    Vector2 lastPos;
    int lastMode;
    bool lock;
    bool initPos;
    TileSet *tileSet;
    Camera2D camera;
    RenderTexture renderTexture;
public:
    EditorInterfaceService();
    void update();
    void draw();
    void unload();
};

#endif
