#ifndef _RPGPP_TILE_H
#define _RPGPP_TILE_H

#include <raylib.h>

class Tile {
private:
    Texture *texture;
    Vector2 atlasCoords;
public:
    Tile(Texture *texture, Vector2 atlasCoords);
    Vector2 getAtlasCoords();
    void draw(Vector2 worldCoords);
};

#endif
