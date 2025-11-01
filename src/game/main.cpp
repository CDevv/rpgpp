#include <raylib.h>
#include "game.hpp"

int main()
{
    InitWindow(640, 480, "RPG++ Game.");
    InitAudioDevice();

    Game game;
    game.init();

    Game::useBin("game.bin");

    //Game::getWorld().setRoom("resources/map.rmap");

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
    CloseAudioDevice();

    return 0;
}
