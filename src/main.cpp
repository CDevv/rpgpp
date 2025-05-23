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

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Hello!", 20, 20, 36, BLACK);

        tileSet.getTile((Vector2){ 0, 0 }).draw((Vector2){ 20, 70 });

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
