#ifndef _RPGPP_TILESET_H
#define _RPGPP_TILESET_H

#include <string>
#include <raylib.h>
#include "atlasTile.hpp"

class TileSet {
private:
    Texture texture;
    int tileSize;
public:
    TileSet(std::string fileName);
    TileSet(Texture texture, int tileSize);
    void unload();
    int getTileSize();
    Texture getTexture();
    AtlasTile getTile(Vector2 atlasPos);
};

#endif
