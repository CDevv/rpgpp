#ifndef _RPGPP_TILESET_H
#define _RPGPP_TILESET_H

#include "gamedata.hpp"
#include <string>
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "atlasTile.hpp"

class TileSet {
private:
    Texture texture;
    Vector2 tileSize;
    std::string textureSource;
public:
    TileSet(std::string fileName);
    TileSet(std::string textureSource, Vector2 tileSize);
    TileSet(std::string textureSource, int tileSizeInt);
    TileSet(Texture texture, Vector2 tileSize);
    TileSet(TileSetBin bin);
    ~TileSet();
    json dumpJson();
    void unload();
    Vector2 getTileSize();
    void setTileSize(int size);
    void setTileSizeVector(Vector2 vec);
    Texture getTexture();
    std::string getTextureSource();
    void setTextureSource(std::string source);
    bool areAtlasCoordsValid(Vector2 atlasCoords);
    AtlasTile getTile(Vector2 atlasCoords);
};

#endif
