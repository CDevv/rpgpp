#include "atlasTile.hpp"
#include <raylib.h>

AtlasTile::AtlasTile() {
    this->texture = nullptr;
    this->atlasCoords = Vector2 { 0 };
}

AtlasTile::AtlasTile(Texture *texture, Vector2 atlasCoords)
{
    this->texture = texture;
    this->atlasCoords = atlasCoords;
}

Vector2 AtlasTile::getAtlasCoords() const
{
    return this->atlasCoords;
}
