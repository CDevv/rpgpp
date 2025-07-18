#ifndef _RPGPP_INTERACTABLE_H
#define _RPGPP_INTERACTABLE_H

#include <raylib.h>

enum InteractableType {
    INT_BLANK,
    INT_TWO
};

class Interactable {
private:
    bool valid;
    InteractableType type;
    Vector2 tilePos;
    int tileSize;
    Vector2 absolutePos;
    Rectangle rect;
public:
    Interactable();
    Interactable(InteractableType type, Vector2 tilePos, int tileSize);
    bool isValid();
    virtual ~Interactable();
    Rectangle getRect();
    InteractableType getType();
    Vector2 getWorldPos();
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
