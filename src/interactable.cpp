#include "interactable.hpp"
#include "game.hpp"
#include <stdio.h>

Interactable::Interactable()
{
    this->valid = false;
}

Interactable::Interactable(InteractableType type, Vector2 tilePos, int tileSize)
{
    this->valid = true;
    this->type = type;
    this->tilePos = tilePos;
    this->tileSize = tileSize;
    this->absolutePos = Vector2 { 0, 0 };

    rect = Rectangle {
        tilePos.x * tileSize, tilePos.y * tileSize,
        static_cast<float>(tileSize), static_cast<float>(tileSize)
    };
}

Interactable::~Interactable()
{
}

bool Interactable::isValid()
{
    return this->valid;
}

Rectangle Interactable::getRect()
{
    return this->rect;
}

Vector2 Interactable::getWorldPos()
{
    return this->tilePos;
}

InteractableType Interactable::getType()
{
    return this->type;
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
    bool value = Game::getState().getProp("test");
    printf("state test: %s\n", value ? "true" : "false");
}

