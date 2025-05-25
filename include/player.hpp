#ifndef _RPGPP_PLAYER_H
#define _RPGPP_PLAYER_H

#include "actor.hpp"
#include <raylib.h>

class Player {
private:
    Actor *actor;
    Vector2 position;
    Direction currentDirection;
    Direction idleDirection;
public:
    Player(Actor *actor);
    void unload();
    void update();
    void draw();
    void moveByVelocity(Vector2 velocity);
};

#endif
