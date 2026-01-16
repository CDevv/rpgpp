#include "TGUI/Widgets/MenuBar.hpp"
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <cstdio>

void run_app() {
  SetTargetFPS(60);

  tgui::Gui gui;

  auto menu = tgui::MenuBar::create();
  menu->addMenu("File");
  menu->addMenuItem("New Project..");

  menu->connectMenuItem({"File", "New Project.."}, [] {
    printf("File:New Project \n");
  });

  gui.add(menu);

  while (!WindowShouldClose()) {
    gui.handleEvents(); // Handles all non-keyboard events

    while (int pressedChar = GetCharPressed())
        gui.handleCharPressed(pressedChar);

    while (int pressedKey = GetKeyPressed())
        gui.handleKeyPressed(pressedKey);

    BeginDrawing();

    ClearBackground(RAYWHITE);

    gui.draw();

    EndDrawing();
  }
}

int main() {
  const int width = 1280;
  const int height = 720;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(width, height, "Editor [TGUI]");

  Image iconImage = LoadImage("resources/app-icon.png");
  SetWindowIcon(iconImage);

  run_app();

  CloseWindow();

  return 0;
}
