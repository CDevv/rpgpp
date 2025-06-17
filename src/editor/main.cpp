#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "editor.hpp"

int main()
{
    const int width = 1280;
    const int height = 640;

    InitWindow(width, height, "editor");

    Editor editor;
    editor.init();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        editor.update();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        editor.draw();

        EndDrawing();
    }

    editor.unload();
    CloseWindow();

    return 0;
}
