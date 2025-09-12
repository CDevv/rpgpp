#ifndef _RPGPP_INTERACTABLE_H
#define _RPGPP_INTERACTABLE_H

#include <raylib.h>

/** ENum for interactable types */
enum InteractableType {
    INT_BLANK,
    INT_TWO
};

/** Defines an object that is interactable in-game by a player's action */
class Interactable {
private:
    /** Whether this Interactable is valid */
    bool valid;
    /** The type of this Interactable */
    InteractableType type;
    /** The position of this Interactable in terms of tiles */
    Vector2 tilePos;
    int tileSize;
    /** The absolute position of this Interactable in the world scene */
    Vector2 absolutePos;
    /** The Rectangle of this Interactable */
    Rectangle rect;
public:
    /** Empty constructor */
    Interactable();
    /** Construct from the type, tile position and size */
    Interactable(InteractableType type, Vector2 tilePos, int tileSize);
    /** Whether this Interactable is valid */
    bool isValid();
    virtual ~Interactable();
    /** Get the Rectangle of this Interactable */
    Rectangle getRect();
    /** Get this Interactable's type */
    InteractableType getType();
    /** Get the World position of this Interactable */
    Vector2 getWorldPos();
    /** Invoke the virtual interact method. Each Interactable has its own interact method. 
     * Shall be invoked when the player interacts with the Interactable */
    virtual void interact();
};

class InteractableOne : public Interactable {
public:
    InteractableOne(Vector2 tilePos, int tileSize);
    void interact() override;
};

class InteractableTwo : public Interactable {
public:
    InteractableTwo(Vector2 tilePos, int tileSize);
    void interact() override;
};

#endif
