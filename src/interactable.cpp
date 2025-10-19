#include "interactable.hpp"
#include <stdio.h>
#include <raylib.h>
#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "interfaceService.hpp"

std::array<std::string, 2> Interactable::interactableTypeNames = {
    "Blank", "Dialogue"
};

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

std::array<std::string, 2>& Interactable::getTypeNames()
{
    return interactableTypeNames;
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

IntBaseWrapper::IntBaseWrapper() {};

IntBaseWrapper::~IntBaseWrapper() {};

void IntBaseWrapper::interact() {};

template <typename T>
void IntBase<T>::interact()
{
    printf("<T>\n");
}

template <>
void IntBase<int>::interact()
{
    printf("Blank \n");
}

template <>
void IntBase<DiagInt>::interact()
{
    Dialogue diag;
    for (Dialogue i : Game::getBin().dialogues) {
        if (i.title == data.dialogueSource) {
            diag = i;
        }
    }

    if (!diag.title.empty()) {
        InterfaceService& ui = Game::getUi();
        ui.showDialogue(diag);
    }
}
