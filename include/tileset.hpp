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
    void setTileSize(int size);
    Texture getTexture();
    std::string getTextureSource();
    void setTextureSource(std::string source);
    AtlasTile getTile(Vector2 atlasCoords);
};

#endif
