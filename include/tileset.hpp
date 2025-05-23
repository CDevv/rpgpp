#ifndef _RPGPP_TILESET_H
#define _RPGPP_TILESET_H

#include <raylib.h>
#include "atlasTile.hpp"

class TileSet {
private:
    Texture texture;
public:
    TileSet(Texture texture);
    ~TileSet();
    Texture getTexture();
    AtlasTile getTile(Vector2 atlasPos);
};

#endif
