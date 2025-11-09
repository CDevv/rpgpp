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
    IntBaseWrapper* interBase;
    InteractablePropsState propsState;
    std::unique_ptr<char[]> diagText;
    char diagChars[125];
    bool diagTextEditMode;
public:
    InteractableInfoPanel();
    InteractableInfoPanel(Rectangle rect);
    void setRect(Rectangle rect);
    void setInitial(InteractableType type);
    void update();
    void draw();
    void setActionMode(RoomAction mode);
    InteractableType getType();
    void setSelected(IntBaseWrapper* inter);
    static void drawTypeProps(InteractableType intType, InteractablePropsState* propsState);
    static void drawDialogueProps(InteractablePropsState* propsState);
    static void drawWarperProps(InteractablePropsState* propsState);
};

#endif
