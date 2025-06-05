#ifndef _RPGPP_TILEMAP_H
#define _RPGPP_TILEMAP_H

#include <string>
#include <vector>
#include <raylib.h>
#include "atlasTile.hpp"
#include "tile.hpp"
#include "tileset.hpp"
#include "interactable.hpp"

class TileMap {
private:
    Vector2 basePos;
    TileSet *tileSet;
    int atlasTileSize;
    int worldTileSize;
    int width;
    int height;
    int maxAtlasWidth;
    int maxAtlasHeight;
    std::vector<std::vector<Tile>> tiles;
    std::vector<Vector2> collisions;
    std::vector<Interactable*> interactables;
public:
    TileMap(std::string fileName);
    TileMap(TileSet *tileSet, int width, int height, int atlasTileSize, int worldTileSize);
    void unload();
    void update();
    void draw();
    int getAtlasTileSize();
    int getWorldTileSize();
    bool atlasPosIsValid(Vector2 atlasPos);
    void setTile(Vector2 worldPos, Vector2 atlasPos);
    void drawTile(int x, int y);
    void drawTile(Vector2 worldPos, Vector2 atlasPos);
    void drawTile(Vector2 worldPos, AtlasTile tile);
    void setCollisionTile(Vector2 worldPos);
    bool isCollisionTile(const Vector2 worldPos);
    std::vector<Vector2> getCollisionTiles();
    void setInteractable(int type, Vector2 worldPos);
    std::vector<Interactable*> getInteractables();
    Vector2 getMaxAtlasSize();
};

#endif
