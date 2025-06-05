#ifndef _RPGPP_INTERACTABLE_H
#define _RPGPP_INTERACTABLE_H

#include <raylib.h>

enum InteractableType {
    INT_BLANK,
    INT_TWO
};

class Interactable {
private:
    InteractableType type;
    Vector2 tilePos;
    int tileSize;
    Vector2 absolutePos;
    Rectangle rect;
public:
    Interactable(InteractableType type, Vector2 tilePos, int tileSize);
    virtual ~Interactable();
    Rectangle getRect();
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
