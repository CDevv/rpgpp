#ifndef _RPGPP_ACTOR_H
#define _RPGPP_ACTOR_H

#include <vector>
#include <array>
#include "atlasTile.hpp"
#include "tileset.hpp"
#include <raylib.h>

enum Direction {
    RPGPP_DOWN_IDLE = 0,
    RPGPP_DOWN = 1,
    RPGPP_UP_IDLE = 2,
    RPGPP_UP = 3,
    RPGPP_LEFT_IDLE = 4,
    RPGPP_LEFT = 5,
    RPGPP_RIGHT_IDLE = 6,
    RPGPP_RIGHT = 7
};

class Actor {
private:
    TileSet *tileSet;
    AtlasTile tile;
    Vector2 position;
    int frameCounter;
    int frameSpeed;
    int currentFrame;
    std::vector<Vector2> *frames;
    std::array<std::vector<Vector2>*, 8> animations;
    Direction currentAnimation;
public:
    Actor(TileSet *tileSet, Vector2 atlasPos);
    void unload();
    void update();
    void draw();
    Vector2 getPosition();
    void setPosition(Vector2 position);
    void moveByVelocity(Vector2 velocity);
    void addAnimation(Direction id, Vector2 atlasPos);
    void changeAnimation(Direction id);
};

#endif
