#include "game.hpp"
#include <raylib.h>

int main() {
  const int width = 640;
  const int height = 480;

  InitWindow(width, height, "Window");

  Game game;
  game.init();

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
