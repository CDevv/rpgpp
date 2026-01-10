#include <raylib.h>
#include "game.hpp"

int main()
{
    InitWindow(640, 480, "RPG++ Game.");
    InitAudioDevice();

    Game game;
    Game::init();

    Game::useBin("game.bin");

    //Game::getWorld().setRoom("resources/map.rmap");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Game::update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Game::draw();
        EndDrawing();
    }

    Game::unload();
    CloseWindow();
    CloseAudioDevice();

    return 0;
}
