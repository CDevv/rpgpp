#include "raylib.h"
#include <imgui.h>
#include <rlImGui.h>
#include <IconsKenney.h>
#include "editor.hpp"

int main()
{
    const int width = 1280;
    const int height = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Editor");

    Editor editor;
    editor.init();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        editor.update();

        BeginDrawing();

        ClearBackground(Color { 20, 20, 20, 255 });

        editor.draw();

        EndDrawing();
    }

    editor.unload();
    CloseWindow();

    return 0;
}
