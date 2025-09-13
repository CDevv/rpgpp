#ifndef _RPGPP_INTERACTABLESCONTAINER_H
#define _RPGPP_INTERACTABLESCONTAINER_H

#include "interactable.hpp"
#include <vector>
#include <memory>

/** Container of Interactables that is to be used by a [Room](Room.md) */
class InteractablesContainer {
private:
    /** The vector, containing all Interactables */
    std::vector<std::unique_ptr<Interactable>> vec;
public:
    /** Empty constructor */
    InteractablesContainer();
    /** Add a new Interactable with tile position and type */
    void add(int x, int y, InteractableType type);
    /** Get an Interactable by its tile position */
    Interactable* get(int x, int y);
    /** Remove an Interactable by its tile position */
    void removeInteractable(int x, int y);
    /** Change the Interactable's type at the specified tile position */
    void setInteractableType(int x, int y, InteractableType type);
    /** Get the vector that contains all Interactables */
    std::vector<Interactable*> getVector();
};

#endif