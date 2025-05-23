#ifndef _RPGPP_TILEMAP_H
#define _RPGPP_TILEMAP_H

#include <raylib.h>
#include "tile.hpp"
#include "tileset.hpp"

class TileMap {
private:
    Vector2 basePos;
    TileSet *tileSet;
    int atlasTileSize;
    int worldTileSize;
    int maxAtlasWidth;
    int maxAtlasHeight;
public:
    TileMap(TileSet *tileSet, int atlasTileSize, int worldTileSize);
    bool atlasPosIsValid(Vector2 atlasPos);
    void drawTile(Vector2 worldPos, Vector2 atlasPos);
    void drawTile(Vector2 worldPos, Tile tile);
    Vector2 getMaxAtlasSize();
};

#endif
