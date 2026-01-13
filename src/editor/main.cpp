#include "raylib.h"
#include <imgui.h>
#include <rlImGui.h>
#include <IconsKenney.h>
#include "editor.hpp"

#include <TGUI/Core.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

void run_app()
{
    tgui::Gui gui;
    auto btn = tgui::Button::create("Test..");
    gui.add(btn, "button");

    while (!WindowShouldClose())
    {
        gui.handleEvents(); // Handles all non-keyboard events

        while (int pressedChar = GetCharPressed())
            gui.handleCharPressed(pressedChar);

        while (int pressedKey = GetKeyPressed())
            gui.handleKeyPressed(pressedKey);

        BeginDrawing();
        ClearBackground({240, 240, 240, 255});
        gui.draw();
        EndDrawing();
    }
}

int main()
{
    const int width = 1280;
    const int height = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(width, height, "Editor [Dear ImGui]");

    Image iconImage = LoadImage("resources/app-icon.png");
    SetWindowIcon(iconImage);

    run_app();

    CloseWindow();

    /*
    const int width = 1280;
    const int height = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Editor [Dear ImGui]");

    Image iconImage = LoadImage("resources/app-icon.png");
    SetWindowIcon(iconImage);

    Editor editor;
    editor.init();

    SetTargetFPS(60);

    rlImGuiSetup(true);

    ImGui::GetIO().WantCaptureKeyboard = true;

    ImGui::GetIO().Fonts->ClearFonts();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("resources/LanaPixel.ttf", 13.0f);

    ImFontConfig config;
    config.MergeMode = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };

    ImGui::GetIO().Fonts->AddFontFromFileTTF("resources/Kenney-Icon-Font.ttf", 13.0f, &config, icon_ranges);

    while (!WindowShouldClose()) {
        editor.update();

        BeginDrawing();
        rlImGuiBegin();

        ClearBackground(Color { 20, 20, 20, 255 });

        editor.draw();

        rlImGuiEnd();
        EndDrawing();
    }

    editor.unload();
    rlImGuiShutdown();
    CloseWindow();

    return 0;
    */
}
