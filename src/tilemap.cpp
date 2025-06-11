#include "tilemap.hpp"
#include "atlasTile.hpp"
#include <raylib.h>
#include <raymath.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

TileMap::TileMap(std::string fileName)
{
    this->basePos = Vector2 { 0.0f, 0.0f };

    char *jsonContent = LoadFileText(fileName.c_str());
    json j = json::parse(jsonContent);

    std::string tileSetSource = j.at("tileset");
    TileSet *tileSet = new TileSet(tileSetSource);
    this->tileSet = tileSet;

    this->atlasTileSize = tileSet->getTileSize();
    int worldTileSize = j.at("tileSize");
    this->worldTileSize = worldTileSize;

    this->width = j.at("width");
    this->height = j.at("height");

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
    auto tileRows = j.at("map");
    size_t rowsCount = tileRows.size();
    for (int64_t y = 0; y < rowsCount; y++) {
        std::vector<std::vector<int>> tileCol = j.at("map").at(y);
        size_t colCount = tileCol.size();

        for (int x = 0; x < colCount; x++) {
            int atlasX = tileCol.at(x).at(0);
            int atlasY = tileCol.at(x).at(1);

            Vector2 worldPos = Vector2 { static_cast<float>(x), static_cast<float>(y) };
            Vector2 atlasPos = Vector2 { static_cast<float>(atlasX), static_cast<float>(atlasY) };

            this->setTile(worldPos, atlasPos);
        }
    }

    //Set collisions..
    std::vector<std::vector<int>> collisionPositions = j.at("collision");
    for (std::vector<int> pos : collisionPositions) {
        int x = pos.at(0);
        int y = pos.at(1);

        Vector2 collisionPos = Vector2{ static_cast<float>(x), static_cast<float>(y) };

        this->setCollisionTile(collisionPos);
    }

    //Set interactables
    std::vector<std::vector<int>> interactablePositions = j.at("interactables");
    for (std::vector<int> pos : interactablePositions) {
        int x = pos.at(0);
        int y = pos.at(1);
        int type = pos.at(2);

        Vector2 interactablePos = Vector2 { static_cast<float>(x), static_cast<float>(y) };
        this->setInteractable(type, interactablePos);
    }

    UnloadFileText(jsonContent);
}

TileMap::TileMap(TileSet *tileSet, int width, int height, int atlasTileSize, int worldTileSize) {
    this->basePos = Vector2 { 0.0f, 0.0f };
    this->tileSet = tileSet;

    this->atlasTileSize = tileSet->getTileSize();
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

void TileMap::unload()
{
    tileSet->unload();
    delete tileSet;

    for (Interactable *interactable : interactables) {
        delete interactable;
    }
}

void TileMap::update()
{
    //this->draw();
}

void TileMap::draw()
{
    //draw the tiles
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            drawTile(x, y);
        }
    }

    //draw collision boxes
    Color collisionDebugColor = RED;
    collisionDebugColor.a = (255 / 2);

    for (Vector2 v : collisions) {
        Rectangle rec = Rectangle {
            v.x * worldTileSize, v.y * worldTileSize,
            static_cast<float>(worldTileSize), static_cast<float>(worldTileSize)
        };

        DrawRectangleRec(rec, collisionDebugColor);
    }

    //draw interactables..
    Color interactableDebugColor = YELLOW;
    interactableDebugColor.a = (255 / 2);

    for (Interactable *interactable : interactables) {
        Rectangle rec = interactable->getRect();
        DrawRectangleRec(rec, interactableDebugColor);
    }
}

int TileMap::getAtlasTileSize()
{
    return atlasTileSize;
}

int TileMap::getWorldTileSize()
{
    return worldTileSize;
}

void TileMap::setTile(Vector2 worldPos, Vector2 atlasPos)
{
    Vector2 resultAtlasCoords = Vector2 {
        atlasPos.x * atlasTileSize,
        atlasPos.y * atlasTileSize
    };

    AtlasTile atlasTile = tileSet->getTile(resultAtlasCoords);

    this->tiles[(int)worldPos.x][(int)worldPos.y].place(atlasTile, worldPos);
}

void TileMap::drawTile(int x, int y)
{
    Tile tile = this->tiles[x][y];

    if (tile.isPlaced()) {
        drawTile(tile.getWorldCoords(), tile.getAtlasTile());
    }
}

bool TileMap::atlasPosIsValid(Vector2 atlasPos)
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

void TileMap::drawTile(Vector2 worldPos, Vector2 atlasPos)
{
    if (!atlasPosIsValid(atlasPos)) {
        return;
    }

    Vector2 resultVector = Vector2 {
        atlasPos.x * atlasTileSize,
        atlasPos.y * atlasTileSize
    };

    AtlasTile tile = tileSet->getTile(resultVector);

    this->drawTile(worldPos, tile);
}

void TileMap::drawTile(Vector2 worldPos, AtlasTile tile)
{
    //defaults..
    const Vector2 origin = Vector2 { 0.0f, 0.0f };
    const float rotation = 0.0f;

    //texture..
    Texture texture = this->tileSet->getTexture();

    //actual coordinates
    Vector2 atlasCoords = tile.getAtlasCoords();
    Vector2 worldCoords = Vector2 {
        this->basePos.x + (worldPos.x * worldTileSize),
        this->basePos.y + (worldPos.y * worldTileSize)
    };

    //Build rects
    Rectangle atlasCoordsRect = Rectangle { atlasCoords.x, atlasCoords.y, static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize) };
    Rectangle worldCoordsRect = Rectangle { worldCoords.x, worldCoords.y, static_cast<float>(worldTileSize), static_cast<float>(worldTileSize) };

    //draw it
    DrawTexturePro(texture, atlasCoordsRect, worldCoordsRect, origin, rotation, WHITE);
}

void TileMap::setCollisionTile(Vector2 worldPos)
{
    this->collisions.push_back(worldPos);
}

bool TileMap::isCollisionTile(const Vector2 worldPos)
{
    for (Vector2 v : collisions) {
        if (Vector2Equals(v, worldPos)) {
            return true;
        }
    }
    return false;
}

std::vector<Vector2> TileMap::getCollisionTiles()
{
    return this->collisions;
}

void TileMap::setInteractable(int type, Vector2 worldPos)
{
    InteractableType interactableType = InteractableType(type);
    Interactable *interactable;

    switch (interactableType) {
        case INT_BLANK:
            interactable = new InteractableOne(worldPos, worldTileSize);
            interactables.push_back(interactable);
            break;
        case INT_TWO:
            interactable = new InteractableTwo(worldPos, worldTileSize);
            interactables.push_back(interactable);
            break;
    };
    //this->interactables.push_back(interactable);
}

std::vector<Interactable*> TileMap::getInteractables()
{
    return this->interactables;
}

Vector2 TileMap::getMaxAtlasSize() {
    Vector2 result = Vector2 { static_cast<float>(maxAtlasWidth), static_cast<float>(maxAtlasHeight) };
    return result;
}
