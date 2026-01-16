#include "editorGuiService.hpp"
#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/MenuBar.hpp"
#include "editor.hpp"
#include "gui_screen.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include "welcome_screen.hpp"
#include <cmath>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>

/*
    Properties of the actual window.
*/
constexpr int BASE_WINDOW_WIDTH = 800;
constexpr int BASE_WINDOW_HEIGHT = 600;
/*
    Properties of the gradient.
*/
constexpr float GRADIENT_SPEED_MUTLIPLIER = 0.3f;
constexpr float GRADIENT_COLOR_MULTIPLIER = 40.0f;

editor_gui_service::editor_gui_service() = default;
void editor_gui_service::initialize_gui() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT, "RPG++ Editor");
  this->reset_gui();
}

void editor_gui_service::reset_gui() {
  this->reset_gui_r = false;
  // Check if we already have a gui already setup, if we do.. don't load the
  // same assets again.
  if (this->current_gui == nullptr) {
    this->current_gui = std::make_unique<tgui::Gui>();
    editor::current_editor->set_editor_icon(RPGPP_EXECUTABLE_LOGO);
  }
  tgui::Theme::setDefault(CURRENT_TESTING_THEME);
  // Remove all the current widgets, and move onto the next process.
  this->current_gui->removeAllWidgets();
  this->create_top_menu_bar();
  this->set_screen_to(std::make_unique<screens::welcome_screen>());
}

void editor_gui_service::start_gui_loop() {
  auto const &cg = this->current_gui;
  SetTraceLogLevel(LOG_WARNING);
  // main loop.
  while (!WindowShouldClose()) {
    cg->handleEvents();
    while (int pressed_char = GetCharPressed())
      cg->handleCharPressed(pressed_char);
    while (int pressedKey = GetKeyPressed())
      cg->handleKeyPressed(pressedKey);
    BeginDrawing();
    ClearBackground(DARKGRAY);
    // Achieve that effect of the gradient.
    auto top_gradient_color = static_cast<unsigned char>(
        abs(sin(GetTime() * GRADIENT_SPEED_MUTLIPLIER)) *
        GRADIENT_COLOR_MULTIPLIER);
    // Draw the gradient.
    DrawRectangleGradientV(
        0, 0, GetRenderWidth(), GetRenderHeight(),
        {top_gradient_color, top_gradient_color, top_gradient_color, 255},
        {40, 40, 40, 255});
    cg->draw();
    // Due to many reasons, one time... Thefirey33 decided to talk to the C++
    // MSVC Compiler if he can reset the current state of his world at any time.
    // MSVC replied, "MEMORY EXCEPTION". Thefirey33 cried, pleaded... his
    // niko-like eyes looking at MSVC. But the solution was... to put a boolean
    // to tell the loop to reset it with a if statement.
    // NOTE: If you think of a better solution. Too Bad!
    if (reset_gui_r)
      this->reset_gui();
    EndDrawing();
  }
}

void editor_gui_service::set_screen_to(
    std::unique_ptr<gui_screen> set_to_screen) {
  // swap the screen pointer to the new screen.
  this->current_screen.swap(set_to_screen);
  this->current_screen->add_elements(this->current_gui.get());
}

void editor_gui_service::create_top_menu_bar() {
  // Clear if there's data left over.
  this->translation_name_and_source.clear();
  auto current_menu_bar = tgui::MenuBar::create();
  auto const &ts = editor::current_editor->ed_translation_service;
  // TODO: File/Project Options.
  current_menu_bar->addMenu(ts->get_translation_by_key("file.options"));
  current_menu_bar->addMenuItem(ts->get_translation_by_key("file.new_project"));
  current_menu_bar->addMenuItem(
      ts->get_translation_by_key("file.open_project"));
  // Translation Options.
  const auto current_menu_text =
      ts->get_translation_by_key("translations.options");
  current_menu_bar->addMenu(current_menu_text);
  for (auto const &[language_file_name, data] : ts->translations) {
    const auto menu_item_text =
        ts->get_translation_by_key("language", language_file_name.c_str());
    current_menu_bar->addMenuItem(menu_item_text);
    // Add data to there if needed.
    this->translation_name_and_source.try_emplace(menu_item_text,
                                                  language_file_name);
  }
  // This allows the user to change the language.
  // FIXME: please make this NOT use a std::map.
  current_menu_bar->onMenuItemClick.connect(
      [this, &ts](const tgui::String &button_text) {
        const auto it = this->translation_name_and_source.find(button_text);
        if (it != this->translation_name_and_source.end()) {
          ts->current_language = it->second;
          this->reset_after_render();
        }
      });
  // TODO: About Options.
  current_menu_bar->addMenu(ts->get_translation_by_key("about.options"));
  current_menu_bar->addMenuItem(
      ts->get_translation_by_key("about.options.rgpp"));
  current_menu_bar->setSize({"100%", "30"});
  current_menu_bar->setMouseCursor(tgui::Cursor::Type::Hand);
  this->current_gui->add(current_menu_bar);
}
