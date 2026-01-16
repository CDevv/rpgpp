#ifndef _RGPP_EDITOR_GUI_CONTAINER_H
#define _RGPP_EDITOR_GUI_CONTAINER_H

#include "TGUI/Backend/raylib.hpp"
#include "screens/guiScreen.hpp"
#include <memory>
// the RPG++ executable logo.
constexpr const char *RPGPP_EXECUTABLE_LOGO = "resources/app-icon.png";
// the current theme file to use.
// TODO: Add theme switching.
constexpr const char *CURRENT_TESTING_THEME = "resources/themes/RPG2007.txt";
// The top margin for the menu.
constexpr int TOP_MARGIN = 35;

class EditorGuiService {
public:
  // gui service constructor.
  bool reset_gui_r = false;
  EditorGuiService();
  std::unique_ptr<tgui::Gui> gui;
  std::unique_ptr<UIScreen> currentScreen;
  // this stores all the current languages in a map, so when we click on the
  // menu.. we can set the language.
  std::map<std::string, std::string, std::less<>> translations =
      {};
  // initialize the gui and register all the components.
  void init();
  // the gui loop starter.
  void uiLoop();
  // this function creates the top menu for RPGPP.
  void initMenuBar();
  void setScreen(std::unique_ptr<UIScreen> newScreen);
  // Reset the current gui session. This allows TGUI to reload assets...
  void resetUi();
  void setResetUi() { this->reset_gui_r = true; }
};
#endif