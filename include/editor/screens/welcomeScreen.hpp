#ifndef _RGPP_WELCOME_SCREEN_H
#define _RGPP_WELCOME_SCREEN_H
#include "editor.hpp"
#include "guiScreen.hpp"

namespace screens {
class WelcomeScreen : public UIScreen {

public:
  void addElement(tgui::Gui *layout) override;
};
} // namespace screens
#endif