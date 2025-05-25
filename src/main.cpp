#include "actor.hpp"
#include "room.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include <raylib.h>

int main()
{
    const int width = 640;
    const int height = 480;

    InitWindow(width, height, "Window");

    Texture texture = LoadTexture("resources/Hills.png");
    TileSet tileSet(texture, 16);

    TileMap tileMap(&tileSet, 50, 50, 16, 48);

    Texture actorTexture = LoadTexture("resources/character.png");
    TileSet actorTileSet(actorTexture, 16);

    Actor *actor = new Actor (&actorTileSet, (Vector2){ 0, 0 });

    Room room(&tileMap);

    room.addActor(actor);

    Vector2 maxSize = tileMap.getMaxAtlasSize();

    for (int x = 0; x < maxSize.x; x++) {
        for (int y = 0; y < maxSize.y; y++) {
            Vector2 vector = (Vector2){ (float)x, (float)y };
            tileMap.setTile(vector, vector);
        }
    }

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
