#ifndef _RPGPP_PROP_H
#define _RPGPP_PROP_H

#include <raylib.h>

class Prop {
private:
    Vector2 worldPos;
    Rectangle atlasRect;
    Texture2D texture;
    Rectangle collisionRect;
public:
    Prop();
    Prop(Rectangle atlasRect, Vector2 worldPos);
    void setTexture(Texture2D texture);
    void setCollisionRect(Rectangle collisionRect);
    Vector2 getWorldPos();
    Rectangle getWorldCollisionRect();
    Vector2 getCollisionCenter();
    void draw();
};

#endif
