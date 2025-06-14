#ifndef _RPGPP_PLAYER_H
#define _RPGPP_PLAYER_H

#include "room.hpp"
#include "actor.hpp"
#include <raylib.h>

class Room;

class Player {
private:
    Room& room;
    Actor *actor;
    Vector2 position;
    float size;
    Vector2 velocity;
    Direction currentDirection;
    Direction idleDirection;
    Rectangle interactableArea;
    void handleCollision();
    void handleInteraction();
public:
    Player(Actor *actor, Room& room);
    void unload();
    void update();
    void draw();
    void setRoom(Room& room);
    void moveByVelocity(Vector2 velocity);
};

#endif
