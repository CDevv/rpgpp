#ifndef _RPGPP_EDITOR_INTERACTABLEINFOPANEL_H
#define _RPGPP_EDITOR_INTERACTABLEINFOPANEL_H

#include "worldViewBox.hpp"
#include <raylib.h>
#include "interactable.hpp"
#include "interactablePropsState.hpp"
#include <memory>

class InteractableInfoPanel {
private:
    Rectangle rect;
    RoomAction action;
    InteractableType lastType;
    InteractableType type;
    Vector2 interactableWorldPos;
    int typeNumber;
    bool typeDropdownEditMode;
    Interactable* interactable;
    InteractablePropsState propsState;
    std::unique_ptr<char[]> diagText;
    bool diagTextEditMode;
    void drawTypeProps();
    void drawDialogueProps();
    void saveProps();
public:
    InteractableInfoPanel();
    InteractableInfoPanel(Rectangle rect);
    void setRect(Rectangle rect);
    void setInitial(InteractableType type);
    void update();
    void draw();
    void setActionMode(RoomAction mode);
    InteractableType getType();
    void setSelectedInteractable(Interactable* interactable);
};

#endif