#ifndef _RPGPP_INTERFACEVIEW_H
#define _RPGPP_INTERFACEVIEW_H

#include "uiElement.hpp"
#include <raylib.h>
#include <memory>
#include <vector>

class InterfaceView {
private:
    Rectangle rect;
    std::vector<std::unique_ptr<UIElement>> elements;
public:
    InterfaceView();
    InterfaceView(Rectangle rect);
    void addElement(UIElement* element);
    void update() const;
    void draw();
};

#endif
