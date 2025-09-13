#ifndef _RPGPP_EDITOR_INTERACTABLEINFOPANEL_H
#define _RPGPP_EDITOR_INTERACTABLEINFOPANEL_H

#include "worldViewBox.hpp"
#include <raylib.h>
#include "interactable.hpp"

class InteractableInfoPanel {
private:
    Rectangle rect;
    RoomAction action;
    InteractableType lastType;
    InteractableType type;
    Vector2 interactableWorldPos;
    int typeNumber;
    bool typeDropdownEditMode;
public:
    InteractableInfoPanel();
    InteractableInfoPanel(Rectangle rect);
    void setInitial(InteractableType type);
    void update();
    void draw();
    void setActionMode(RoomAction mode);
    InteractableType getType();
    void setSelectedInteractable(Interactable* interactable);
};

#endif