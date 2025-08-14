#ifndef _RPGPP_EDITOR_PROPERTIESBOX_H
#define _RPGPP_EDITOR_PROPERTIESBOX_H

#include <raylib.h>
#include "tileSetPropertiesBox.hpp"
#include "mapPropertiesBox.hpp"
#include "actorPropertiesBox.hpp"

class PropertiesBox {
private:
    Vector2 pos;
    Rectangle rect;
    TileSetPropertiesBox tileSetProps;
    MapPropertiesBox mapProps;
    ActorPropertiesBox actorProps;
public:
    PropertiesBox();
    PropertiesBox(Rectangle rect);
    void update();
    void draw();
    void setDefaults();
};

#endif
