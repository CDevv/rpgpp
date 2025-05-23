#include "tileset.hpp"
#include "tile.hpp"
#include <raylib.h>

TileSet::TileSet(Texture texture) {
    this->texture = texture;
}

Texture TileSet::getTexture() {
    return this->texture;
}

Tile TileSet::getTile(Vector2 atlasCoords) {
    Tile tile(&this->texture, atlasCoords);

    return tile;
}
