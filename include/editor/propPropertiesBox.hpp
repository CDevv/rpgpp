#ifndef _RPGPP_EDITOR_PROPPROPERTIESBOX_H
#define _RPGPP_EDITOR_PROPPROPERTIESBOX_H

#include "interactable.hpp"
#include "prop.hpp"
#include <raylib.h>
#include <array>

class PropPropertiesBox {
private:
    Prop* prop;
    Rectangle rect;
    bool collisionViewActive;
    std::array<int, 2> rectPos;
    std::array<int, 2> rectSize;
    bool propHasInteractable;
    int intType;
public:
    PropPropertiesBox();
    void setDefaults();
    void update();
    void draw();
    bool getCollisionViewActive();
};

#endif
