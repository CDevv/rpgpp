#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <imgui.h>
#include <rlImGui.h>
#include "editor.hpp"

#include "layoutContainer.hpp"

int main()
{
    const int width = 1280;
    const int height = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Editor [Dear ImGui]");

    Editor editor;
    editor.init();

    SetTargetFPS(60);

    rlImGuiSetup(true);

    char input[256] = "";

    while (!WindowShouldClose()) {
        editor.update();

        BeginDrawing();
        rlImGuiBegin();

        ClearBackground(RAYWHITE);

        editor.draw();

        rlImGuiEnd();
        EndDrawing();
    }

    //editor.unload();
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
