#ifndef _RPGPP_EDITOR_PROPINFOPANEL_H
#define _RPGPP_EDITOR_PROPINFOPANEL_H

#include "interactablePropsState.hpp"
#include "worldViewBox.hpp"
#include <raylib.h>

class PropInfoPanel {
private:
    Rectangle rect;
    RoomAction action;
    std::string propConcat;
    int currentProp;
    InteractablePropsState intPropsState;
public:
    PropInfoPanel();
    PropInfoPanel(Rectangle rect);
    void setRect(Rectangle rect);
    void setActionMode(RoomAction action);
    int getCurrentPropIndex();
    void update();
    void draw();
};

#endif
