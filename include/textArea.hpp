#ifndef _RPGPP_TEXTAREA_H
#define _RPGPP_TEXTAREA_H

#include "uiElement.hpp"
#include <raylib.h>
#include <string>

class TextArea : public UIElement {
private:
    Rectangle rect;
    std::string content;
    void putChar(int i, Vector2 *charPos, Vector2 *charMeasure) const;
public:
    TextArea();
    TextArea(Rectangle rect);
    void setText(const std::string &text);
    void update();
    void draw();
};

#endif
