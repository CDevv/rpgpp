#include "tilemap.hpp"
#include "atlasTile.hpp"
#include "tileset.hpp"
#include <memory>
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

TileMap::TileMap(const std::string& fileName)
{
    this->basePos = Vector2 { 0.0f, 0.0f };

    char *jsonContent = LoadFileText(fileName.c_str());
    json jsonObj = json::parse(jsonContent);

    this->tileSetSource = jsonObj.at("tileset");
    this->tileSet = std::make_unique<TileSet>(tileSetSource);

    this->atlasTileSize = static_cast<int>(tileSet->getTileSize().x);
    this->worldTileSize = jsonObj.at("tileSize");

    this->width = jsonObj.at("width");
    this->height = jsonObj.at("height");

    this->maxAtlasWidth = this->tileSet->getTexture().width / atlasTileSize;
    this->maxAtlasHeight = this->tileSet->getTexture().height / atlasTileSize;

    //Initialize empty tiles
    for (int i = 0; i < width; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < height; j++) {
            Tile tile;
            row.push_back(tile);
        }

        tiles.push_back(row);
    }

    //Place the tiles..
    //this->tiles = std::vector<std::vector<Tile>>{};

    auto tileRows = jsonObj.at("map");
    size_t rowsCount = tileRows.size();
    for (int64_t y = 0; y < rowsCount; y++) {
        std::vector<std::vector<int>> tileCol = jsonObj.at("map").at(y);
        size_t colCount = tileCol.size();

        for (int x = 0; x < colCount; x++) {
            int atlasX = tileCol.at(x).at(0);
            int atlasY = tileCol.at(x).at(1);

            if (atlasX >= 0 && atlasY >= 0) {
                Vector2 worldPos = { static_cast<float>(x), static_cast<float>(y) };
                Vector2 atlasPos = { static_cast<float>(atlasX), static_cast<float>(atlasY) };

                this->setTile(worldPos, atlasPos);
            }
        }
    }

    UnloadFileText(jsonContent);
}

TileMap::TileMap(const std::string& tileSetSource, int width, int height, int atlasTileSize, int worldTileSize) {
    this->basePos = Vector2 { 0.0f, 0.0f };
    this->tileSet = std::make_unique<TileSet>(tileSetSource);
    this->tileSetSource = tileSetSource;

    this->atlasTileSize = static_cast<int>(tileSet->getTileSize().x);
    this->worldTileSize = worldTileSize;

    this->width = width;
    this->height = height;

    this->maxAtlasWidth = tileSet->getTexture().width / atlasTileSize;
    this->maxAtlasHeight = tileSet->getTexture().height / atlasTileSize;

    for (int i = 0; i < width; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < height; j++) {
            Tile tile;
            row.push_back(tile);
        }

        tiles.push_back(row);
    }
}

TileMap::TileMap(const TileSet& tileSet, int width, int height, int atlasTileSize, int worldTileSize)
{
    this->basePos = Vector2 { 0.0f, 0.0f };
    this->tileSet = std::make_unique<TileSet>(tileSet);

    this->atlasTileSize = this->tileSet->getTileSize().x;
    this->worldTileSize = worldTileSize;

    this->width = width;
    this->height = height;

    this->maxAtlasWidth = this->tileSet->getTexture().width / atlasTileSize;
    this->maxAtlasHeight = this->tileSet->getTexture().height / atlasTileSize;

    for (int i = 0; i < width; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < height; j++) {
            Tile tile;
            row.push_back(tile);
        }

        tiles.push_back(row);
    }
}

TileMap::TileMap(std::unique_ptr<TileSet> tileSetPtr, int width, int height, int atlasTileSize, int worldTileSize)
{
    this->basePos = Vector2 { 0.0f, 0.0f };
    this->tileSet = std::move(tileSetPtr);

    this->atlasTileSize = static_cast<int>(this->tileSet->getTileSize().x);
    this->worldTileSize = worldTileSize;

    this->width = width;
    this->height = height;

    this->maxAtlasWidth = this->tileSet->getTexture().width / atlasTileSize;
    this->maxAtlasHeight = this->tileSet->getTexture().height / atlasTileSize;

    for (int i = 0; i < width; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < height; j++) {
            Tile tile;
            row.push_back(tile);
        }

        tiles.push_back(row);
    }
}

TileMap::TileMap(const RoomBin &bin)
{
    this->basePos = Vector2 { 0.0f, 0.0f };
    this->tileSet = std::make_unique<TileSet>(Game::getBin().tilesets.at(bin.tileSetName));
    this->atlasTileSize = this->tileSet->getTileSize().x;
    this->worldTileSize = 48;

    this->width = bin.width;
    this->height = bin.height;

    this->maxAtlasWidth = this->tileSet->getTexture().width / atlasTileSize;
    this->maxAtlasHeight = this->tileSet->getTexture().height / atlasTileSize;

    for (int i = 0; i < width; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < height; j++) {
            Tile tile;
            row.push_back(tile);
        }

        tiles.push_back(row);
    }

    for (auto row : bin.tiles) {
        for (auto col : row) {
            Vector2 atlasPos = Vector2 { static_cast<float>(col.atlasPos.x / tileSet->getTileSize().x), static_cast<float>(col.atlasPos.y / tileSet->getTileSize().y) };
            Vector2 worldPos = Vector2 { static_cast<float>(col.worldPos.x), static_cast<float>(col.worldPos.y) };
            this->setTile(worldPos, atlasPos);
        }
    }
}

json TileMap::dumpJson()
{
    //Make a vector for the tiles
    auto tilesVector = std::vector<std::vector<std::vector<int>>>();
    for (int i = 0; i < height; i++) {
        tilesVector.emplace_back();
        for (int j = 0; j < width; j++) {
            Tile tile = getTile(j, i);
            std::vector<int> atlasPosVector;
            if (tile.isPlaced()) {
                AtlasTile atlasTile = tile.getAtlasTile();
                Vector2 atlasCoords = atlasTile.getAtlasCoords();
                atlasPosVector.push_back(static_cast<int>(atlasCoords.x / atlasTileSize));
                atlasPosVector.push_back(static_cast<int>(atlasCoords.y / atlasTileSize));
            } else {
                atlasPosVector.push_back(-1);
                atlasPosVector.push_back(-1);
            }
            tilesVector[i].push_back(atlasPosVector);
        }
    }

    //Make the json object
    json tileMapJson = {
        {"tileset", tileSetSource},
        {"tileSize", worldTileSize},
        {"width", width},
        {"height", height},
        {"map", tilesVector}
    };

    return tileMapJson;
}

void TileMap::unload() const
{
    tileSet->unload();
}

void TileMap::update()
{
    //this->draw();
}

void TileMap::draw() const
{
    if (tileSet->getTileSize().x != tileSet->getTileSize().y) {
        DrawText("Tile size must be square", 20, 20, 36, RED);
        return;
    }

    //draw the tiles
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            drawTile(x, y);
        }
    }
}

std::string TileMap::getTileSetSource()
{
    return tileSetSource;
}

TileSet* TileMap::getTileSet() const
{
    return tileSet.get();
}

void TileMap::setTileSet(const std::string& tileSetSource)
{
    this->tileSet = std::make_unique<TileSet>(tileSetSource);
    this->tileSetSource = tileSetSource;
}

int TileMap::getAtlasTileSize() const
{
    return atlasTileSize;
}

int TileMap::getWorldTileSize() const
{
    return worldTileSize;
}

Tile TileMap::getTile(int x, int y) const
{
    return this->tiles[x][y];
}

void TileMap::setTile(Vector2 worldPos, Vector2 atlasPos)
{
    Vector2 resultAtlasCoords = {
        atlasPos.x * atlasTileSize,
        atlasPos.y * atlasTileSize
    };

    AtlasTile atlasTile = tileSet->getTile(resultAtlasCoords);

    this->tiles[static_cast<int>(worldPos.x)][static_cast<int>(worldPos.y)].place(atlasTile, worldPos);
}

void TileMap::setEmptyTile(Vector2 worldPos)
{
    this->tiles[static_cast<int>(worldPos.x)][static_cast<int>(worldPos.y)].erase();
}

void TileMap::drawTile(int x, int y) const
{
    Tile tile = this->tiles[x][y];

    if (tile.isPlaced()) {
        drawTile(tile.getWorldCoords(), tile.getAtlasTile());
    }
}

bool TileMap::atlasPosIsValid(Vector2 atlasPos) const
{
    bool atlasXFits = false;
    bool atlasYFits = false;

    if (atlasPos.x >= 0 && atlasPos.x <= maxAtlasWidth) {
        atlasXFits = true;
    }
    if (atlasPos.y >= 0 && atlasPos.y <= maxAtlasHeight) {
        atlasYFits = true;
    }

    return (atlasXFits && atlasYFits);
}

void TileMap::drawTile(Vector2 worldPos, Vector2 atlasPos) const
{
    if (!atlasPosIsValid(atlasPos)) {
        return;
    }

    Vector2 resultVector = {
        atlasPos.x * atlasTileSize,
        atlasPos.y * atlasTileSize
    };

    AtlasTile tile = tileSet->getTile(resultVector);

    this->drawTile(worldPos, tile);
}

void TileMap::drawTile(Vector2 worldPos, AtlasTile tile) const
{
    //defaults..
    constexpr Vector2 origin = Vector2 { 0.0f, 0.0f };
    constexpr float rotation = 0.0f;

    //texture..
    Texture texture = this->tileSet->getTexture();

    //actual coordinates
    Vector2 atlasCoords = tile.getAtlasCoords();
    Vector2 worldCoords = {
        this->basePos.x + (worldPos.x * static_cast<float>(worldTileSize)),
        this->basePos.y + (worldPos.y * static_cast<float>(worldTileSize))
    };

    //Build rects
    Rectangle atlasCoordsRect = { atlasCoords.x, atlasCoords.y, static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize) };
    Rectangle worldCoordsRect = { worldCoords.x, worldCoords.y, static_cast<float>(worldTileSize), static_cast<float>(worldTileSize) };

    //draw it
    DrawTexturePro(texture, atlasCoordsRect, worldCoordsRect, origin, rotation, WHITE);
}

Vector2 TileMap::getMaxAtlasSize() const
{
    Vector2 result = Vector2 { static_cast<float>(maxAtlasWidth), static_cast<float>(maxAtlasHeight) };
    return result;
}

Vector2 TileMap::getMaxWorldSize() const
{
    Vector2 result = Vector2 { static_cast<float>(width), static_cast<float>(height) };
    return result;
}

void TileMap::setWorldSize(Vector2 size)
{
    if (size.x < width) {
        int diff = (width - size.x);
        for (int i = 0; i < diff; i++) {
            tiles.pop_back();
        }
    } else if (size.x > width) {
        int diff = (size.x - width);
        for (int i = 0; i < diff; i++) {
            std::vector<Tile> row;
            for (int j = 0; j < size.y; j++) {
                Tile tile;
                row.push_back(tile);
            }
            tiles.push_back(row);
        }
    }

    if (size.y < height) {
        int diff = (height - size.y);
        for (int x = 0; x < tiles.size(); x++) {
            for (int y = 0; y < diff; y++) {
                tiles[x].pop_back();
            }
        }
    } else if (size.y > height) {
        int diff = (size.y - height);
        for (int x = 0; x < tiles.size(); x++) {
            for (int y = 0; y < diff; y++) {
                Tile tile;
                tiles[x].push_back(tile);
            }
        }
    }

    this->width = size.x;
    this->height = size.y;
}

