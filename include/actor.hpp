#ifndef _RPGPP_ACTOR_H
#define _RPGPP_ACTOR_H

#include <vector>
#include "atlasTile.hpp"
#include "tileset.hpp"
#include <raylib.h>

class Actor {
private:
    TileSet *tileSet;
    AtlasTile tile;
    Vector2 position;
    int frameCounter;
    int frameSpeed;
    int currentFrame;
    std::vector<Vector2> frames;
public:
    Actor(TileSet *tileSet, Vector2 atlasPos);
    void unload();
    void update();
    void draw();
};

#endif
