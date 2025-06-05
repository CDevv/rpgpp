#include "interactable.hpp"
#include <stdio.h>

Interactable::Interactable(InteractableType type, Vector2 tilePos, int tileSize)
{
    this->type = type;
    this->tilePos = tilePos;
    this->tileSize = tileSize;

    rect = (Rectangle){
        tilePos.x * tileSize, tilePos.y * tileSize,
        (float)tileSize, (float)tileSize
    };
}

Interactable::~Interactable()
{
}

Rectangle Interactable::getRect()
{
    return this->rect;
}

void Interactable::interact()
{
    if (type == INT_BLANK) {
        printf("interaction!\n");
    } else {
        printf("two.\n");
    }
}

InteractableOne::InteractableOne(Vector2 tilePos, int tileSize)
: Interactable(INT_BLANK, tilePos, tileSize)
{
}

void InteractableOne::interact()
{
    printf("one test.\n");
}

InteractableTwo::InteractableTwo(Vector2 tilePos, int tileSize)
: Interactable(INT_TWO, tilePos, tileSize)
{
}

void InteractableTwo::interact()
{
    printf("two test.\n");
}

