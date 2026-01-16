#ifndef _RPGPP_BASE_GUI_WIDGET_H
#define _RPGPP_BASE_GUI_WIDGET_H
#include "TGUI/Backend/raylib.hpp"
class gui_screen {

public:
  virtual ~gui_screen() = default;
  gui_screen() = default;

  virtual void add_elements(tgui::Gui *layout) {
    // this is a base method, meaning other objects should extend it to create
    // widgets.
  }
  virtual void on_screen_switch(tgui::Gui *layout) {
    // When the screen switches,
    // The screen should unload some of it's stuff here.
  }
};
#endif