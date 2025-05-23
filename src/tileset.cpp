#include "tileset.hpp"
#include "tile.hpp"
#include <raylib.h>

TileSet::TileSet(Texture texture) {
    this->texture = texture;
}

Tile TileSet::getTile(Vector2 atlasCoords) {
    Tile tile(&this->texture, atlasCoords);

    return tile;
}
