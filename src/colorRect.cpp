#include "colorRect.hpp"
#include <raylib.h>

ColorRect::ColorRect() : rect(Rectangle { 1, 1, 1, 1 }), color()
{
}

ColorRect::ColorRect(Rectangle rect)
{
    this->rect = rect;
    this->color = RAYWHITE;
}

void ColorRect::setColor(Color newColor)
{
    this->color = newColor;
}

void ColorRect::update()
{

}

void ColorRect::draw()
{
    DrawRectangleRec(rect, color);
}
