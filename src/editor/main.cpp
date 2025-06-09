#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "editor.hpp"

int main()
{
    const int width = 640;
    const int height = 480;

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
