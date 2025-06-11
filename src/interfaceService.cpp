#include "interfaceService.hpp"

InterfaceService::InterfaceService()
{
    fpsVisible = false;

    Font font = LoadFontEx("resources/monogram-extended.ttf", 32, 0, 250);
    this->font = font;
}

void InterfaceService::update()
{
    if (IsKeyPressed(KEY_Q)) {
        fpsVisible = !fpsVisible;
    }
}

void InterfaceService::draw()
{
    if (fpsVisible) {
        DrawFPS(10, 10);
        DrawTextEx(font, "rpgpp", Vector2 { 10, 36 }, (float)font.baseSize, 2, RED);
    }
}

void InterfaceService::unload()
{
    UnloadFont(font);
}
