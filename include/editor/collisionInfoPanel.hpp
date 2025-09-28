#ifndef _RPGPP_EDITOR_COLLISIONINFOPANEL_H
#define _RPGPP_EDITOR_COLLISIONINFOPANEL_H

#include <raylib.h>
#include "worldViewBox.hpp"

class CollisionInfoPanel {
private:
    Rectangle rect;
    RoomAction action;
public:
    CollisionInfoPanel();
    CollisionInfoPanel(Rectangle rect);
    void setRect(Rectangle rect);
    void draw();
    void setActionMode(RoomAction mode);
};

#endif