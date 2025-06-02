#include "actor.hpp"
#include "room.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "player.hpp"
#include <raylib.h>

#include <nlohmann/json.hpp>
#include <stdio.h>

int main()
{
    const int width = 640;
    const int height = 480;

    InitWindow(width, height, "Window");

    TileMap tileMap("resources/map.json");

    TileSet actorTileSet("resources/actorTiles.json");

    Actor actor(&actorTileSet, (Vector2){ 0, 0 });

    actor.addAnimation(RPGPP_DOWN_IDLE, (Vector2){ 0, 0 });
    actor.addAnimation(RPGPP_DOWN_IDLE, (Vector2){ 1, 0 });

    actor.addAnimation(RPGPP_DOWN, (Vector2){ 2, 0 });
    actor.addAnimation(RPGPP_DOWN, (Vector2){ 3, 0 });

    actor.addAnimation(RPGPP_UP_IDLE, (Vector2){ 0, 1 });
    actor.addAnimation(RPGPP_UP_IDLE, (Vector2){ 1, 1 });

    actor.addAnimation(RPGPP_UP, (Vector2){ 2, 1 });
    actor.addAnimation(RPGPP_UP, (Vector2){ 3, 1 });

    actor.addAnimation(RPGPP_LEFT_IDLE, (Vector2){ 0, 2 });
    actor.addAnimation(RPGPP_LEFT_IDLE, (Vector2){ 1, 2 });

    actor.addAnimation(RPGPP_LEFT, (Vector2){ 2, 2 });
    actor.addAnimation(RPGPP_LEFT, (Vector2){ 3, 2 });

    actor.addAnimation(RPGPP_RIGHT_IDLE, (Vector2){ 0, 3 });
    actor.addAnimation(RPGPP_RIGHT_IDLE, (Vector2){ 1, 3 });

    actor.addAnimation(RPGPP_RIGHT, (Vector2){ 2, 3 });
    actor.addAnimation(RPGPP_RIGHT, (Vector2){ 3, 3 });

    Player player(&actor);

    Room room(&tileMap);

    room.addPlayer(&player);

    //tileMap.setCollisionTile((Vector2){ 0, 3 });
    //tileMap.setCollisionTile((Vector2){ 1, 3 });

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
