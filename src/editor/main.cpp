#include "raylib.h"

#include <TGUI/Core.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

void run_app()
{
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

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
