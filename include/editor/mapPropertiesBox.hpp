#ifndef _RPGPP_EDITOR_MAPPROPERTIESBOX_H
#define _RPGPP_EDITOR_MAPPROPERTIESBOX_H

#include <raylib.h>
#include "room.hpp"
#include "tilemap.hpp"

class MapPropertiesBox {
private:
    Rectangle rect;
    Room *room;
    TileMap *map;
    int mapHeight;
    bool heightEdit;
    int mapWidth;
    bool widthEdit;
    bool musicOn;
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
