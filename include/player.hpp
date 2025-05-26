#ifndef _RPGPP_PLAYER_H
#define _RPGPP_PLAYER_H

#include "room.hpp"
#include "actor.hpp"
#include <raylib.h>

class Room;

class Player {
private:
    Room *room;
    Actor *actor;
    Vector2 position;
    float size;
    Direction currentDirection;
    Direction idleDirection;
public:
    Player(Actor *actor);
    void unload();
    void update();
    void draw();
    void setRoom(Room *room);
    void moveByVelocity(Vector2 velocity);
};

#endif
