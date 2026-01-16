#include "editorGuiService.hpp"
#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/MenuBar.hpp"
#include "editor.hpp"
#include "guiScreen.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include "welcomeScreen.hpp"
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

EditorGuiService::EditorGuiService() = default;
void EditorGuiService::init() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT, "RPG++ Editor");

  this->resetUi();
}

void EditorGuiService::resetUi() {
  this->reset_gui_r = false;
  // Check if we already have a gui already setup, if we do.. don't load the
  // same assets again.
  if (this->gui == nullptr) {
    this->gui = std::make_unique<tgui::Gui>();
    Editor::instance->setAppIcon(RPGPP_EXECUTABLE_LOGO);
  }
  tgui::Theme::setDefault(CURRENT_TESTING_THEME);

  // Remove all the current widgets, and move onto the next process.
  this->gui->removeAllWidgets();
  this->initMenuBar();
  this->setScreen(std::make_unique<screens::WelcomeScreen>());
}

void EditorGuiService::uiLoop() {
  auto const &cg = this->gui;
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
      this->resetUi();
    EndDrawing();
  }
}

void EditorGuiService::setScreen(std::unique_ptr<UIScreen> set_to_screen) {
  // swap the screen pointer to the new screen.
  this->currentScreen.swap(set_to_screen);
  this->currentScreen->addElement(this->gui.get());
}

void EditorGuiService::initMenuBar() {
  // Clear if there's data left over.
  this->translations.clear();
  auto current_menu_bar = tgui::MenuBar::create();
  auto const &ts = Editor::instance->translationService;
  // TODO: File/Project Options.
  current_menu_bar->addMenu(ts->getKey("file.options"));
  current_menu_bar->addMenuItem(ts->getKey("file.new_project"));
  current_menu_bar->addMenuItem(ts->getKey("file.open_project"));
  // Translation Options.
  const auto current_menu_text = ts->getKey("translations.options");
  current_menu_bar->addMenu(current_menu_text);
  for (auto const &[language_file_name, data] : ts->translations) {
    const auto menu_item_text =
        ts->getKey("language", language_file_name.c_str());
    current_menu_bar->addMenuItem(menu_item_text);
    // Add data to there if needed.
    this->translations.try_emplace(menu_item_text, language_file_name);
  }
  // This allows the user to change the language.
  // FIXME: please make this NOT use a std::map.
  current_menu_bar->onMenuItemClick.connect(
      [this, &ts](const tgui::String &button_text) {
        const auto it = this->translations.find(button_text);
        if (it != this->translations.end()) {
          ts->current_language = it->second;
          this->setResetUi();
        }
      });
  // TODO: About Options.
  current_menu_bar->addMenu(ts->getKey("about.options"));
  current_menu_bar->addMenuItem(ts->getKey("about.options.rgpp"));
  current_menu_bar->setSize({"100%", "30"});
  current_menu_bar->setMouseCursor(tgui::Cursor::Type::Hand);
  this->gui->add(current_menu_bar);
}
