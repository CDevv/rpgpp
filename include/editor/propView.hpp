#ifndef _RPGPP_EDITOR_PROPVIEW_H
#define _RPGPP_EDITOR_PROPVIEW_H

#include <raylib.h>
#include "collisionbox.hpp"

class PropView {
private:
    Rectangle rect;
    Rectangle renderRect;
    Camera2D camera;
    RenderTexture2D renderTexture;
    CollisionBox collisionBox;
    bool collisionBoxActive;
public:
    PropView();
    PropView(Rectangle rect);
    void setInitial();
    void setRect(Rectangle rect);
    void setCollisionBoxActive(bool value);
    void update();
    void draw();
};

#endif
