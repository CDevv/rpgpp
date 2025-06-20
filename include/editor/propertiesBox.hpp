#ifndef _RPGPP_EDITOR_PROPERTIESBOX_H
#define _RPGPP_EDITOR_PROPERTIESBOX_H

#include <raylib.h>
#include "tileSetPropertiesBox.hpp"
#include "mapPropertiesBox.hpp"

class PropertiesBox {
private:
    Vector2 pos;
    Rectangle rect;
    TileSetPropertiesBox tileSetProps;
    MapPropertiesBox mapProps;
public:
    PropertiesBox();
    PropertiesBox(Rectangle rect);
    void update();
    void draw();
    void setDefaults();
};

#endif
