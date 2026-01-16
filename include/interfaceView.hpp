#ifndef _RPGPP_INTERFACEVIEW_H
#define _RPGPP_INTERFACEVIEW_H

#include "uiElement.hpp"
#include <memory>
#include <raylib.h>
#include <vector>

class InterfaceView {
private:
  Rectangle rect;
  std::vector<std::unique_ptr<UIElement>> elements;

public:
  InterfaceView();
  explicit InterfaceView(Rectangle rect);
  void addElement(UIElement *element);
  void update() const;
  void draw() const;
};

#endif
