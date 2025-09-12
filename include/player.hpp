#ifndef _RPGPP_PLAYER_H
#define _RPGPP_PLAYER_H

class Room;

#include "room.hpp"
#include "actor.hpp"
#include <raylib.h>

/** THe Player class represents the player character that the player controls. */
class Player {
private:
    /** Reference to the Room that the Player is currently in. */
    Room& room;
    /** The Actor that this Player controls. */
    std::unique_ptr<Actor> actor;
    /** The absolute position of this Player. */
    Vector2 position;
    float size;
    /** The change in the player's position. */
    Vector2 velocity;
    /** Current direction state of the Player. */
    Direction currentDirection;
    Direction idleDirection;
    Rectangle interactableArea;
    /** Handle the collision for this Player. */
    void handleCollision();
    /** Handle the interactions for this Player. */
    void handleInteraction();
public:
    /** Construct from an Actor and the Room where the Player shall be contained. */
    Player(std::unique_ptr<Actor> actor, Room& room);
    /** Unload routine. */
    void unload();
    /** Update routine. */
    void update();
    /** Draw routine. */
    void draw();
    /** Set the Player's current room. */
    void setRoom(Room& room);
    /** Move the player by a certain velocity. */
    void moveByVelocity(Vector2 velocity);
};

#endif
