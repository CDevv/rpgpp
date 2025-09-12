#include "interfaceService.hpp"

InterfaceService::InterfaceService()
{
    fpsVisible = false;

    Font font = LoadFontEx("LanaPixel.ttf", 13, 0, 250);
    this->font = font;

    Image img = LoadImage("ui-npatch.png");
    ImageResizeNN(&img, img.width * 3, img.height * 3);

    this->uiTexture = LoadTextureFromImage(img);

    Rectangle destRec = Rectangle {
        0, 0, static_cast<float>(GetScreenWidth() - 20), 140
    };
    destRec.x = (GetScreenWidth() - destRec.width) / 2;
    destRec.y = (GetScreenHeight() - destRec.height) - 20;
    this->dialogue = DialogueBalloon(destRec);
}

InterfaceService::~InterfaceService()
{
    UnloadFont(font);
    UnloadTexture(this->uiTexture);
}

Font InterfaceService::getFont()
{
    return font;
}

Texture InterfaceService::getTexture()
{
    return uiTexture;
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

    dialogue.draw();
}

void InterfaceService::drawScaledNPatch(Texture2D texture, NPatchInfo info, Rectangle destRec, int scale)
{
    
}

void InterfaceService::unload()
{
    //UnloadFont(font);
}
