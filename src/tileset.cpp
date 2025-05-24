#include "tileset.hpp"
#include "atlasTile.hpp"
#include <raylib.h>

TileSet::TileSet(Texture texture, int tileSize) {
    this->texture = texture;
    this->tileSize = tileSize;
}

TileSet::~TileSet() {
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
