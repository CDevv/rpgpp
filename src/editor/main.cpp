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
    Editor editor;
    editor.init();

    SetTargetFPS(60);

    /*
    ImGui::GetIO().WantCaptureKeyboard = true;

    ImGui::GetIO().Fonts->ClearFonts();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("resources/LanaPixel.ttf", 13.0f);

    ImFontConfig config;
    config.MergeMode = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };

    ImGui::GetIO().Fonts->AddFontFromFileTTF("resources/Kenney-Icon-Font.ttf", 13.0f, &config, icon_ranges);
    */

    while (!WindowShouldClose()) {
        editor.update();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        editor.draw();

        EndDrawing();
    }

    editor.unload();
}

int main()
{
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
