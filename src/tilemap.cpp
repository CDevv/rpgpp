#include "tilemap.hpp"
#include "tile.hpp"

TileMap::TileMap(TileSet *tileSet, int atlasTileSize, int worldTileSize) {
    this->basePos = (Vector2){ 0.0f, 0.0f };
    this->tileSet = tileSet;

    this->atlasTileSize = atlasTileSize;
    this->worldTileSize = worldTileSize;

    this->maxAtlasWidth = tileSet->getTexture().width / atlasTileSize;
    this->maxAtlasHeight = tileSet->getTexture().height / atlasTileSize;
}

bool TileMap::atlasPosIsValid(Vector2 atlasPos) {
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

void TileMap::drawTile(Vector2 worldPos, Vector2 atlasPos) {
    if (!atlasPosIsValid(atlasPos)) {
        return;
    }

    Vector2 resultVector = (Vector2){
        atlasPos.x * atlasTileSize,
        atlasPos.y * atlasTileSize
    };

    Tile tile = tileSet->getTile(resultVector);

    this->drawTile(worldPos, tile);
}

void TileMap::drawTile(Vector2 worldPos, Tile tile) {
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

Vector2 TileMap::getMaxAtlasSize() {
    Vector2 result = (Vector2){ (float)maxAtlasWidth, (float)maxAtlasHeight };
    return result;
}
