#ifndef _RPGPP_TILEMAP_H
#define _RPGPP_TILEMAP_H

#include "game.hpp"
#include "gamedata.hpp"
#include <memory>
#include <string>
#include <vector>
#include <raylib.h>
#include "atlasTile.hpp"
#include "tile.hpp"
#include "tileset.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/** A TileMap represents a map of the placed tiles in a Room. */
class TileMap {
private:
    /** Base position of this TileMap. */
    Vector2 basePos;
    /** File path to the source of the used TileSet. */
    std::string tileSetSource;
    /** A unique_ptr to the used TileSet. */
    std::unique_ptr<TileSet> tileSet;
    int atlasTileSize;
    int worldTileSize;
    /** Width of the TileMap in tiles */
    int width;
    /** Height of the TileMap in height */
    int height;
    int maxAtlasWidth;
    int maxAtlasHeight;
    /** A collection representing the tiles in this map */
    std::vector<std::vector<Tile>> tiles;
public:
    /** Construct from a file name/path */
    TileMap(std::string fileName);
    /** Construct using a TileSet path, width, height */
    TileMap(std::string tileSetSource, int width, int height, int atlasTileSize, int worldTileSize);
    /** Constructor using a TileSet object */
    TileMap(TileSet tileSet, int width, int height, int atlasTileSize, int worldTileSize);
    /** Constructor using a unique_ptr of a TileSet */
    TileMap(std::unique_ptr<TileSet> tileSetPtr, int width, int height, int atlasTileSize, int worldTileSize);
    /** Constructor using a RoomBin binary structure */
    TileMap(RoomBin bin);
    /** Dump the TileMap's data into a nlohmann::json object */
    json dumpJson();
    /** Unload routine. Typically calls raylib's UnloadTexture. */
    void unload();
    /** Update routine. */
    void update();
    /** Draw routine. */
    void draw();
    /** Get the path of the TileSet resource */
    std::string getTileSetSource();
    /** Get a pointer to the used TileSet */
    TileSet* getTileSet();
    /** Set a new TileSet for this TileMap using a path for the new TileSet */
    void setTileSet(std::string tileSetSource);
    int getAtlasTileSize();
    int getWorldTileSize();
    bool atlasPosIsValid(Vector2 atlasPos);
    /** Get the Tile at the specified x and y position */
    Tile getTile(int x, int y);
    /** Set the tile at the specified world position to be a tile at the specified atlas position from the TileSet */
    void setTile(Vector2 worldPos, Vector2 atlasPos);
    /** Set the tile at the specified world position to be empty (not drawn) */
    void setEmptyTile(Vector2 worldPos);
    /** Draw the tile at the specified world x and y coordinates */
    void drawTile(int x, int y);
    void drawTile(Vector2 worldPos, Vector2 atlasPos);
    void drawTile(Vector2 worldPos, AtlasTile tile);
    Vector2 getMaxAtlasSize();
    /** Get the size of this TileMap
     * @returns the size in a Vector2. The x component is the width; the y component is the height.
     */
    Vector2 getMaxWorldSize();
    /** Set the size of this TileSet using the provided Vector2 argument. The x ocmponent will be the new width; the y component will be the new height. */
    void setWorldSize(Vector2 size);
};

#endif
