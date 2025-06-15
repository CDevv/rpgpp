#include <raylib.h>
#include "game.hpp"

int main()
{
    InitWindow(640, 480, "RPG++ Game.");

    Game game;
    game.init();

    Game::getWorld().setRoom("resources/map.json");

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
