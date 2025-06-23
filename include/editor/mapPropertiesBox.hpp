#ifndef _RPGPP_EDITOR_MAPPROPERTIESBOX_H
#define _RPGPP_EDITOR_MAPPROPERTIESBOX_H

#include <raylib.h>
#include "tilemap.hpp"

class MapPropertiesBox {
private:
    Rectangle rect;
    TileMap *map;
    int mapHeight;
    bool heightEdit;
    int mapWidth;
    bool widthEdit;
    Vector2 scrollVec;
    Rectangle viewRec;
public:
    MapPropertiesBox();
    MapPropertiesBox(Rectangle rect);
    void update();
    void draw();
    void setDefaults();
};

#endif
