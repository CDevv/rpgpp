#include "tileset.hpp"
#include "atlasTile.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

TileSet::TileSet(std::string textureSource, int tileSize)
{
    this->textureSource = textureSource;
    Texture texture = LoadTexture(textureSource.c_str());
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
    this->textureSource = sourcePath;
    Texture texture = LoadTexture(sourcePath.c_str());
    this->texture = texture;

    //Set tile size
    int tileSize = j.at("tileSize");
    this->tileSize = tileSize;

    //Unload file's text
    UnloadFileText(jsonContent);
}

TileSet::~TileSet()
{
    UnloadTexture(texture);
}

json TileSet::dumpJson()
{
    json tileSetJson = {
        {"source", textureSource},
        {"tileSize", tileSize}
    };

    return tileSetJson;
}

void TileSet::unload()
{
    UnloadTexture(texture);
}

int TileSet::getTileSize()
{
    return this->tileSize;
}

void TileSet::setTileSize(int size)
{
    this->tileSize = size;
}

Texture TileSet::getTexture()
{
    return this->texture;
}

std::string TileSet::getTextureSource()
{
    return this->textureSource;
}

void TileSet::setTextureSource(std::string source)
{
    this->textureSource = source;
    this->texture = LoadTexture(source.c_str());
}

bool TileSet::areAtlasCoordsValid(Vector2 atlasCoords)
{
    bool xValid = false;
    bool yValid = false;
    bool posValid = false;

    if (atlasCoords.x >= 0 && atlasCoords.x < (texture.width / static_cast<float>(tileSize))) {
        xValid = true;
    }
    if (atlasCoords.y >= 0 && atlasCoords.y < (texture.height / static_cast<float>(tileSize))) {
        yValid = true;
    }

    posValid = (xValid && yValid);
    return posValid;
}

AtlasTile TileSet::getTile(Vector2 atlasCoords)
{
    AtlasTile tile(&this->texture, atlasCoords);

    return tile;
}
