#include "interactable.hpp"
#include <stdio.h>
#include <raylib.h>
#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "interfaceService.hpp"

std::array<std::string, INTTYPE_MAX> Interactable::interactableTypeNames = {
    "Blank", "Dialogue", "Warper"
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

std::array<std::string, INTTYPE_MAX>& Interactable::getTypeNames()
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
    if (Game::getBin().dialogues.count(data.dialogueSource)) {
        InterfaceService& ui = Game::getUi();
        ui.showDialogue(Game::getBin().dialogues[data.dialogueSource]);
    }
}

template <>
void IntBase<WarperInt>::interact()
{
    WorldService& world = Game::getWorld();
    world.setRoom(data.targetRoom);
}

std::unique_ptr<IntBaseWrapper> make_inter_item(Vector2 pos, InteractableType type)
{
    switch (type) {
        case INT_BLANK:
            return std::unique_ptr<IntBaseWrapper>(new IntBase<int>(pos, type));
            break;
        case INT_TWO:
            return std::unique_ptr<IntBaseWrapper>(new IntBase<DiagInt>(pos, type));
            break;
        case INT_WARPER:
            return std::unique_ptr<IntBaseWrapper>(new IntBase<WarperInt>(pos, type));
            break;
    }
}

void inter_apply_vec(IntBaseWrapper* inter, std::vector<std::string> props)
{
    if (inter->type != INT_BLANK && props.size() == 0) {
        props.push_back("");
    }

    if (inter->type == INT_TWO) {
        DiagInt diagInt;
        diagInt.dialogueSource = props.at(0);

        (static_cast<IntBase<DiagInt>*>(inter))->set(diagInt);
    }
    if (inter->type == INT_WARPER) {
        (static_cast<IntBase<WarperInt>*>(inter))->set({props.at(0)});
    }
}

void inter_apply_bin(IntBaseWrapper* inter, InteractableBin intBin)
{
    IntBase<DiagInt>* diag;
    switch (intBin.type) {
    case INT_TWO:
        diag = static_cast<IntBase<DiagInt>*>(inter);
        diag->set(DiagInt {intBin.dialogue});
        break;
    case INT_WARPER:
        static_cast<IntBase<WarperInt>*>(inter)->set({
            intBin.dialogue
        });
        break;
    default:
        break;
    }
}
