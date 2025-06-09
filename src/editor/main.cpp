#include <raylib.h>
#include "game.hpp"

int main()
{
    const int width = 640;
    const int height = 480;

    InitWindow(width, height, "editor");

    Game game;
    game.init();

    Game::getWorld()->setRoom("resources/map.json");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game.update();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        game.draw();

        EndDrawing();
    }

    game.unload();
    CloseWindow();

    return 0;
}
