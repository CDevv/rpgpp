#include "room.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include <raylib.h>

int main()
{
    const int width = 640;
    const int height = 480;

    InitWindow(width, height, "Window");

    const Vector2 origin = (Vector2){ 0.0f, 0.0f };
    const float rotation = 0.0f;

    Texture texture = LoadTexture("resources/Hills.png");

    TileSet tileSet(texture);
    TileMap tileMap(&tileSet, 50, 50, 16, 48);

    Room room(&tileMap);

    Vector2 maxSize = tileMap.getMaxAtlasSize();

    for (int x = 0; x < maxSize.x; x++) {
        for (int y = 0; y < maxSize.y; y++) {
            Vector2 vector = (Vector2){ (float)x, (float)y };
            tileMap.setTile(vector, vector);
        }
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        room.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
