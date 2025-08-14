#ifndef _RPGPP_EDITOR_TILESETPROPERTIESBOX_H
#define _RPGPP_EDITOR_TILESETPROPERTIESBOX_H

#include <raylib.h>
#include "tileset.hpp"

class TileSetPropertiesBox {
private:
    Rectangle rect;
    TileSet *tileSet;
    int chosenTileSize;
    int chosenTileWidth;
    int chosenTileHeight;
    bool multiSizeCheckBox;
    bool chosenTileSizeEditMode;
    bool chosenTileWidthEditMode;
    bool chosenTileHeightEditMode;
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
