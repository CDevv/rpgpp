#ifndef _RPGPP_EDITOR_TILESETPROPERTIESBOX_H
#define _RPGPP_EDITOR_TILESETPROPERTIESBOX_H

#include "tileset.hpp"

class TileSetPropertiesBox {
private:
    int chosenTileSize;
    bool chosenTileSizeEditMode;
    TileSet *tileSet;
public:
    TileSetPropertiesBox();
    void update();
    void draw();
    void setDefaults();
};

#endif
