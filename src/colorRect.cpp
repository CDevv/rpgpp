#include "colorRect.hpp"
#include <raylib.h>

ColorRect::ColorRect() {}

ColorRect::ColorRect(Rectangle rect)
{
    this->rect = rect;
    this->color = RAYWHITE;
}

void ColorRect::setColor(Color color)
{
    this->color = color;
}

void ColorRect::update()
{

}

void ColorRect::draw()
{
    DrawRectangleRec(rect, color);
}
