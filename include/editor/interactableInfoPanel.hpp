#ifndef _RPGPP_EDITOR_INTERACTABLEINFOPANEL_H
#define _RPGPP_EDITOR_INTERACTABLEINFOPANEL_H

#include "worldViewBox.hpp"
#include <raylib.h>

class InteractableInfoPanel {
private:
    Rectangle rect;
    RoomAction action;
public:
    InteractableInfoPanel();
    InteractableInfoPanel(Rectangle rect);
    void draw();
    void setActionMode(RoomAction mode);
};

#endif