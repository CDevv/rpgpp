#ifndef _RPGPP_EDITOR_PROPERTIESBOX_H
#define _RPGPP_EDITOR_PROPERTIESBOX_H

#include <raylib.h>
#include "tileset.hpp"

class PropertiesBox {
private:
    Vector2 pos;
    Rectangle rect;
    int chosenTileSize;
    bool chosenTileSizeEditMode;
    TileSet *tileSet;
public:
    PropertiesBox();
    PropertiesBox(Rectangle rect);
    void update();
    void draw();
};

#endif
