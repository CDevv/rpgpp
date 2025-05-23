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
    TileMap tileMap(&tileSet, 16, 48);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //DrawText("Hello!", 20, 20, 36, BLACK);

        Vector2 maxSize = tileMap.getMaxAtlasSize();

        for (int x = 0; x < maxSize.x; x++) {
            for (int y = 0; y < maxSize.y; y++) {
                Vector2 vector = (Vector2){ (float)x, (float)y };
                tileMap.drawTile(vector, vector);
            }
        }

        //tileMap.drawTile((Vector2){ 0, 0 }, (Vector2){ 0, 0 });
        //tileMap.drawTile((Vector2){ 0, 1 }, (Vector2){ 0, 1 });
        //tileMap.drawTile((Vector2){ 0, 2 }, (Vector2){ 0, 2 });

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
