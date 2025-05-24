#ifndef _RPGPP_TILESET_H
#define _RPGPP_TILESET_H

#include <raylib.h>
#include "atlasTile.hpp"

class TileSet {
private:
    Texture texture;
    int tileSize;
public:
    TileSet(Texture texture, int tileSize);
    ~TileSet();
    int getTileSize();
    Texture getTexture();
    AtlasTile getTile(Vector2 atlasPos);
};

#endif
