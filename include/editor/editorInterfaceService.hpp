#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <raylib.h>
#include <string>
#include "tileset.hpp"

class EditorInterfaceService {
private:
    bool openDialog;
    char *tilePath;
    bool openedTileSet;
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
