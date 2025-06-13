#ifndef _RPGPP_TILESET_H
#define _RPGPP_TILESET_H

#include <string>
#include <raylib.h>
#include "atlasTile.hpp"

class TileSet {
private:
    Texture texture;
    int tileSize;
    std::string textureSource;
public:
    TileSet(std::string fileName);
    TileSet(Texture texture, int tileSize);
    ~TileSet();
    void unload();
    int getTileSize();
    Texture getTexture();
    std::string getTextureSource();
    AtlasTile getTile(Vector2 atlasCoords);
};

#endif
