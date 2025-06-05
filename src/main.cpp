#include "actor.hpp"
#include "room.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "player.hpp"
#include <raylib.h>

int main()
{
    const int width = 640;
    const int height = 480;

    InitWindow(width, height, "Window");

    TileMap tileMap("resources/map.json");

    Actor actor("resources/playerActor.json");

    Player player(&actor);

    Room room(&tileMap);

    room.addPlayer(&player);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        room.update();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        room.draw();

        EndDrawing();
    }

    room.unload();

    CloseWindow();

    return 0;
}
