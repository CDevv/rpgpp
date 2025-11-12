#ifndef _RPGPP_IMAGERECT_H
#define _RPGPP_IMAGERECT_H

#include <raylib.h>
#include "uiElement.hpp"
#include <string>

class ImageRect : public UIElement {
private:
    Rectangle rect;
    std::string source;
    Texture2D texture;
public:
    ImageRect();
    ImageRect(Rectangle rect);
    void setSource(std::string source);
    void setTexture(Texture2D texture);
    void update();
    void draw();
};

#endif
