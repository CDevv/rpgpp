#include "tileset.hpp"
#include "atlasTile.hpp"
#include <raylib.h>

TileSet::TileSet(Texture texture) {
    this->texture = texture;
}

TileSet::~TileSet() {
    UnloadTexture(texture);
}

Texture TileSet::getTexture() {
    return this->texture;
}

AtlasTile TileSet::getTile(Vector2 atlasCoords) {
    AtlasTile tile(&this->texture, atlasCoords);

    return tile;
}
