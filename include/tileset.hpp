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
    int tileSize;
    std::string textureSource;
public:
    TileSet(std::string fileName);
    TileSet(std::string textureSource, int tileSize);
    TileSet(Texture texture, int tileSize);
    TileSet(TileSetBin bin);
    ~TileSet();
    json dumpJson();
    void unload();
    int getTileSize();
    void setTileSize(int size);
    Texture getTexture();
    std::string getTextureSource();
    void setTextureSource(std::string source);
    bool areAtlasCoordsValid(Vector2 atlasCoords);
    AtlasTile getTile(Vector2 atlasCoords);
};

#endif
