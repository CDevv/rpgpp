#include "tileset.hpp"
#include "atlasTile.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

TileSet::TileSet(std::string textureSource, Vector2 tileSize)
{
    this->textureSource = textureSource;
    Texture texture = LoadTexture(textureSource.c_str());
    this->texture = texture;
    this->tileSize = tileSize;
}

TileSet::TileSet(std::string textureSource, int tileSizeInt)
: TileSet(textureSource, Vector2 { static_cast<float>(tileSizeInt), static_cast<float>(tileSizeInt) })
{
}

TileSet::TileSet(Texture texture, Vector2 tileSize)
{
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
    //this->tileSize = tileSize;

    int tileWidth = j.at("tileWidth");
    int tileHeight = j.at("tileHeight");
    this->tileSize = Vector2 { static_cast<float>(tileWidth), static_cast<float>(tileHeight) };

    //Unload file's text
    UnloadFileText(jsonContent);
}

TileSet::TileSet(TileSetBin bin)
{
    this->tileSize = Vector2 { static_cast<float>(bin.tileSize.x), static_cast<float>(bin.tileSize.y) };

    unsigned char* imageData = bin.image.data();
    Image image = LoadImageFromMemory(bin.extension.c_str(), imageData, bin.dataSize);
    Texture texture = LoadTextureFromImage(image);

    this->texture = texture;
}

TileSet::~TileSet()
{
    //UnloadTexture(texture);
}

json TileSet::dumpJson()
{
    json tileSetJson = {
        {"source", textureSource},
        {"tileSize", tileSize.x},
        {"tileWidth", tileSize.x},
        {"tileHeight", tileSize.y}
    };

    return tileSetJson;
}

void TileSet::unload()
{
    UnloadTexture(texture);
}

Vector2 TileSet::getTileSize()
{
    return this->tileSize;
}

void TileSet::setTileSize(int size)
{
    this->tileSize = Vector2 { static_cast<float>(size), static_cast<float>(size) };
}

void TileSet::setTileSizeVector(Vector2 vec)
{
    this->tileSize = vec;
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

    if (atlasCoords.x >= 0 && atlasCoords.x < (texture.width / static_cast<float>(tileSize.x))) {
        xValid = true;
    }
    if (atlasCoords.y >= 0 && atlasCoords.y < (texture.height / static_cast<float>(tileSize.y))) {
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
