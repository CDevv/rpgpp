#include "tileset.hpp"
#include "atlasTile.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

TileSet::TileSet(Texture texture, int tileSize) {
    this->texture = texture;
    this->tileSize = tileSize;
}

TileSet::TileSet(std::string fileName)
{
    //Get text from JSON file
    char *jsonContent = LoadFileText(fileName.c_str());
    json j = json::parse(jsonContent);

    //Load Texture
    std::string sourcePath = j.at("source");
    Texture texture = LoadTexture(sourcePath.c_str());
    this->texture = texture;

    //Set tile size
    int tileSize = j.at("tileSize");
    this->tileSize = tileSize;

    //Unload file's text
    UnloadFileText(jsonContent);
}

void TileSet::unload() {
    UnloadTexture(texture);
}

int TileSet::getTileSize() {
    return tileSize;
}

Texture TileSet::getTexture() {
    return this->texture;
}

AtlasTile TileSet::getTile(Vector2 atlasCoords) {
    AtlasTile tile(&this->texture, atlasCoords);

    return tile;
}
