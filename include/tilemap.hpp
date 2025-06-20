#ifndef _RPGPP_TILEMAP_H
#define _RPGPP_TILEMAP_H

#include <memory>
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
    std::string tileSetSource;
    std::unique_ptr<TileSet> tileSet;
    int atlasTileSize;
    int worldTileSize;
    int width;
    int height;
    int maxAtlasWidth;
    int maxAtlasHeight;
    std::vector<std::vector<Tile>> tiles;
    std::vector<Vector2> collisions;
    std::unique_ptr<std::vector<Interactable>> interactables;
public:
    TileMap(std::string fileName);
    TileMap(std::unique_ptr<TileSet> tileSet, int width, int height, int atlasTileSize, int worldTileSize);
    void unload();
    void update();
    void draw();
    std::string getTileSetSource();
    TileSet& getTileSet();
    void setTileSet(std::string tileSetSource);
    int getAtlasTileSize();
    int getWorldTileSize();
    bool atlasPosIsValid(Vector2 atlasPos);
    Tile getTile(int x, int y);
    void setTile(Vector2 worldPos, Vector2 atlasPos);
    void drawTile(int x, int y);
    void drawTile(Vector2 worldPos, Vector2 atlasPos);
    void drawTile(Vector2 worldPos, AtlasTile tile);
    void setCollisionTile(Vector2 worldPos);
    bool isCollisionTile(const Vector2 worldPos);
    std::vector<Vector2> getCollisionTiles();
    void setInteractable(int type, Vector2 worldPos);
    std::vector<Interactable>& getInteractables();
    Vector2 getMaxAtlasSize();
    Vector2 getMaxWorldSize();
    void setWorldSize(Vector2 size);
};

#endif
