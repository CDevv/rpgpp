#include "tilemap.hpp"
#include "atlasTile.hpp"
#include <raylib.h>
#include <raymath.h>

TileMap::TileMap(TileSet *tileSet, int width, int height, int atlasTileSize, int worldTileSize) {
    this->basePos = (Vector2){ 0.0f, 0.0f };
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
    UnloadTexture(tileSet->getTexture());
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
        Rectangle rec = (Rectangle){
            v.x * worldTileSize, v.y * worldTileSize,
            (float)worldTileSize, (float)worldTileSize
        };

        DrawRectangleRec(rec, collisionDebugColor);
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
    Vector2 resultAtlasCoords = (Vector2){
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

    Vector2 resultVector = (Vector2){
        atlasPos.x * atlasTileSize,
        atlasPos.y * atlasTileSize
    };

    AtlasTile tile = tileSet->getTile(resultVector);

    this->drawTile(worldPos, tile);
}

void TileMap::drawTile(Vector2 worldPos, AtlasTile tile)
{
    //defaults..
    const Vector2 origin = (Vector2){ 0.0f, 0.0f };
    const float rotation = 0.0f;

    //texture..
    Texture texture = this->tileSet->getTexture();

    //actual coordinates
    Vector2 atlasCoords = tile.getAtlasCoords();
    Vector2 worldCoords = (Vector2){
        this->basePos.x + (worldPos.x * worldTileSize),
        this->basePos.y + (worldPos.y * worldTileSize)
    };

    //Build rects
    Rectangle atlasCoordsRect = (Rectangle){ atlasCoords.x, atlasCoords.y, (float)atlasTileSize, (float)atlasTileSize };
    Rectangle worldCoordsRect = (Rectangle){ worldCoords.x, worldCoords.y, (float)worldTileSize, (float)worldTileSize };

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

Vector2 TileMap::getMaxAtlasSize() {
    Vector2 result = (Vector2){ (float)maxAtlasWidth, (float)maxAtlasHeight };
    return result;
}
