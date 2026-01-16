#ifndef _RGPP_EDITOR_GUI_CONTAINER_H
#define _RGPP_EDITOR_GUI_CONTAINER_H

#include "TGUI/Backend/raylib.hpp"
#include "screens/gui_screen.hpp"
#include <memory>
// the RPG++ executable logo.
constexpr const char *RPGPP_EXECUTABLE_LOGO = "resources/app-icon.png";
// the current theme file to use.
// TODO: Add theme switching.
constexpr const char *CURRENT_TESTING_THEME = "resources/themes/RPG2007.txt";
// The top margin for the menu.
constexpr int TOP_MARGIN = 35;

class editor_gui_service {
public:
  // gui service constructor.
  bool reset_gui_r = false;
  editor_gui_service();
  std::unique_ptr<tgui::Gui> current_gui;
  std::unique_ptr<gui_screen> current_screen;
  // this stores all the current languages in a map, so when we click on the
  // menu.. we can set the language.
  std::map<std::string, std::string, std::less<>> translation_name_and_source =
      {};
  // initialize the gui and register all the components.
  void initialize_gui();
  // the gui loop starter.
  void start_gui_loop();
  // this function creates the top menu for RPGPP.
  void create_top_menu_bar();
  void set_screen_to(std::unique_ptr<gui_screen> set_to_screen);
  // Reset the current gui session. This allows TGUI to reload assets...
  void reset_gui();
  void reset_after_render() { this->reset_gui_r = true; }
};
#endif