#ifndef _RPGPP_EDITOR_WORLDVIEWBOX_H
#define _RPGPP_EDITOR_WORLDVIEWBOX_H

#include <raylib.h>
#include "tileset.hpp"

class WorldViewBox {
private:
    Camera2D *camera;
    TileSet *tileSet;
    RenderTexture renderTexture;
    std::string windowTitle;
public:
    WorldViewBox();
    WorldViewBox(Camera2D *camera);
    void draw();
    void unload();
};

#endif
