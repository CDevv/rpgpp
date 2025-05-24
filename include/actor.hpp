#ifndef _RPGPP_ACTOR_H
#define _RPGPP_ACTOR_H

#include "atlasTile.hpp"
#include "tileset.hpp"
#include <raylib.h>

class Actor {
private:
    TileSet *tileSet;
    AtlasTile tile;
    Vector2 position;
public:
    Actor(TileSet *tileSet, Vector2 atlasPos);
    void draw();
};

#endif
