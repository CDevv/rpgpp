#ifndef _RPGPP_TILE_H
#define _RPGPP_TILE_H

#include "atlasTile.hpp"
#include <raylib.h>

class Tile {
private:
    AtlasTile atlasTile;
    Vector2 worldCoords;
    bool placed;
public:
    Tile();
    bool isPlaced();
    void place(AtlasTile atlasTile, Vector2 worldCoords);
    AtlasTile getAtlasTile();
    Vector2 getWorldCoords();
};

#endif
