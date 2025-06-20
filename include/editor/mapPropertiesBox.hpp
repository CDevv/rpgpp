#ifndef _RPGPP_EDITOR_MAPPROPERTIESBOX_H
#define _RPGPP_EDITOR_MAPPROPERTIESBOX_H

#include "tilemap.hpp"

class MapPropertiesBox {
private:
    TileMap *map;
    int mapHeight;
    bool heightEdit;
    int mapWidth;
    bool widthEdit;
public:
    MapPropertiesBox();
    void update();
    void draw();
    void setDefaults();
};

#endif
