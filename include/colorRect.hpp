#ifndef _RPGPP_COLORRECT_H
#define _RPGPP_COLORRECT_H

#include <raylib.h>
#include "uiElement.hpp"

class ColorRect : public UIElement {
private:
    Rectangle rect;
    Color color;
public:
    ColorRect();
    ColorRect(Rectangle rect);
    void setColor(Color newColor);
    void update() override;
    void draw() override;
};

#endif
