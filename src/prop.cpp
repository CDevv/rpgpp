#include "prop.hpp"
#include "gamedata.hpp"
#include <raylib.h>

Prop::Prop() {}

Prop::Prop(Rectangle atlasRect, Vector2 worldPos)
{
    this->atlasRect = atlasRect;
    this->worldPos = worldPos;
    this->collisionRect = Rectangle { 0, 0, 16, 16 };
}

void Prop::setTexture(Texture2D texture)
{
    this->texture = texture;
}

void Prop::setCollisionRect(Rectangle collisionRect)
{
    this->collisionRect = collisionRect;
}

Vector2 Prop::getWorldPos()
{
    return worldPos;
}

Rectangle Prop::getWorldCollisionRect()
{
    return Rectangle {
        worldPos.x + (collisionRect.x * RPGPP_DRAW_MULTIPLIER), worldPos.y + (collisionRect.y * RPGPP_DRAW_MULTIPLIER),
        collisionRect.width * RPGPP_DRAW_MULTIPLIER, collisionRect.height * RPGPP_DRAW_MULTIPLIER
    };
}

Vector2 Prop::getCollisionCenter()
{
    Rectangle rect = getWorldCollisionRect();
    return Vector2 {
        rect.x + (rect.width / 2), rect.y + (rect.height / 2)
    };
}

void Prop::draw()
{
    Rectangle dest = Rectangle {
        worldPos.x, worldPos.y,
        atlasRect.width * RPGPP_DRAW_MULTIPLIER, atlasRect.height * RPGPP_DRAW_MULTIPLIER
    };
    DrawTexturePro(texture, atlasRect, dest, Vector2 { 0, 0 }, 0.0f, WHITE);

    DrawRectangleRec(getWorldCollisionRect(), Fade(RED, 0.5f));
}
