#ifndef _RPGPP_BASE_GUI_WIDGET_H
#define _RPGPP_BASE_GUI_WIDGET_H
#include "TGUI/Backend/raylib.hpp"
class UIScreen {

public:
  virtual ~UIScreen() = default;
  UIScreen() = default;

  virtual void addElement(tgui::Gui *layout) {
    // this is a base method, meaning other objects should extend it to create
    // widgets.
  }
};
#endif