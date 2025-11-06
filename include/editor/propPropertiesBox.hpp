#ifndef _RPGPP_EDITOR_PROPPROPERTIESBOX_H
#define _RPGPP_EDITOR_PROPPROPERTIESBOX_H

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
public:
    PropPropertiesBox();
    void setDefaults();
    void update();
    void draw();
    bool getCollisionViewActive();
};

#endif
