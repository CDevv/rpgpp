#ifndef _RGPP_WELCOME_SCREEN_H
#define _RGPP_WELCOME_SCREEN_H
#include "editor.hpp"
#include "gui_screen.hpp"

namespace screens {
class welcome_screen : public gui_screen {

public:
  void add_elements(tgui::Gui *layout) override;
};
} // namespace screens
#endif