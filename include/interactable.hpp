#ifndef _RPGPP_INTERACTABLE_H
#define _RPGPP_INTERACTABLE_H

#include <raylib.h>
#include <map>
#include <array>
#include <string>

#include "dialogueBalloon.hpp"

/** Enum for interactable types */
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
protected:
    /** Interactable properties. */
    std::map<std::string, std::string> props;
    /** Array for descriptive names of interactable types */
    static std::array<std::string, 2> interactableTypeNames;
public:
    /** Empty constructor */
    Interactable();
    /** Construct from the type, tile position and size */
    Interactable(InteractableType type, Vector2 tilePos, int tileSize);
    /** Get the array containing names of the Interactable names */
    static std::array<std::string, 2>& getTypeNames();
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
    /** Set a property of this interactable. */
    void setProp(std::string key, std::string value);
};

class InteractableOne : public Interactable {
public:
    InteractableOne(Vector2 tilePos, int tileSize);
    void interact() override;
};

class InteractableTwo : public Interactable {
private:
    Dialogue dialogue;
public:
    InteractableTwo(Vector2 tilePos, int tileSize);
    void interact() override;
    Dialogue getDialogue();
    void setDialogue(Dialogue dialogue);
};

#endif
