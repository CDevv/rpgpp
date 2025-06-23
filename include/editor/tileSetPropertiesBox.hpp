#ifndef _RPGPP_EDITOR_TILESETPROPERTIESBOX_H
#define _RPGPP_EDITOR_TILESETPROPERTIESBOX_H

#include <raylib.h>
#include "tileset.hpp"

class TileSetPropertiesBox {
private:
    Rectangle rect;
    TileSet *tileSet;
    int chosenTileSize;
    bool chosenTileSizeEditMode;
    Vector2 scrollVec;
    Rectangle viewRec;
public:
    TileSetPropertiesBox();
    TileSetPropertiesBox(Rectangle rect);
    void update();
    void draw();
    void setDefaults();
};

#endif
