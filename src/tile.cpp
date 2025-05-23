#include "tile.hpp"
#include <raylib.h>

Tile::Tile(Texture *texture, Vector2 atlasCoords) {
    this->texture = texture;
    this->atlasCoords = atlasCoords;
}

void Tile::draw(Vector2 worldCoords) {
    Rectangle atlasCoordsRect = (Rectangle){ this->atlasCoords.x, this->atlasCoords.y, 16, 16 };
    Rectangle worldCoordsRect = (Rectangle){ worldCoords.x, worldCoords.y, 48, 48 };

    const Vector2 origin = (Vector2){ 0.0f, 0.0f };
    const float rotation = 0.0f;

    DrawTexturePro(*this->texture, atlasCoordsRect, worldCoordsRect, origin, rotation, WHITE);
}
