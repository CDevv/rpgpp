#ifndef _RPGPP_TILESET_H
#define _RPGPP_TILESET_H

#include <raylib.h>
#include "tile.hpp"

class TileSet {
private:
    Texture texture;
public:
    TileSet(Texture texture);
    Texture getTexture();
    Tile getTile(Vector2 atlasPos);
};

#endif
