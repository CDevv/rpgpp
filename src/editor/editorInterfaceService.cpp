#include "editorInterfaceService.hpp"
#include <raygui.h>
#include <raymath.h>
#include "nfd.h"

EditorInterfaceService::EditorInterfaceService()
{
    NFD_Init();

    uiFont = LoadFont("resources/ark-pixel-10px-monospaced-latin.otf");

    openDialog = false;
    openedTileSet = false;
    tileSet = nullptr;
    mousePos = (Vector2){ 0, 0 };

    camera = (Camera2D){ {0} };
    camera.target = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    renderTexture = LoadRenderTexture(494, 420);

    GuiLoadStyle("rpgpp.rgs");
}

void EditorInterfaceService::update()
{
    const float change = 2;
    Vector2 velocity = (Vector2){ 0, 0 };

    if (IsKeyDown(KEY_LEFT)) {
        velocity.x -= change;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        velocity.x += change;
    }
    if (IsKeyDown(KEY_UP)) {
        velocity.y -= change;
    }
    if (IsKeyDown(KEY_DOWN)) {
        velocity.y += change;
    }

    Vector2 resultPos = Vector2Add(camera.target, velocity);
    camera.target = resultPos;

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        float scale = 0.2f * wheel;
        camera.zoom = Clamp(camera.zoom + scale, 1.0f, 5.0f);
    }
}

void EditorInterfaceService::draw()
{
    mousePos = Vector2Subtract(GetMousePosition(), (Vector2){ 138, 32 });
    Vector2 hoverPos = Vector2Add(mousePos, Vector2Scale(camera.target, camera.zoom));
    hoverPos = Vector2Scale(hoverPos, (1/camera.zoom));

    Vector2 tileAtlasPos = (Vector2){ 0, 0 };
    Vector2 tileWorldPos = (Vector2){ 0, 0 };
    bool hoverValidX = false;
    bool hoverValidY = false;
    bool hoverValidTile = false;

    if (openedTileSet) {
        Texture tileSetTexture = tileSet->getTexture();
        int tileSize = tileSet->getTileSize();
        int tilesWidth = tileSetTexture.width / tileSet->getTileSize();
        int tilesHeight = tileSetTexture.height / tileSet->getTileSize();

        if (hoverPos.x >= 0 && hoverPos.x <= tileSetTexture.width) {
            hoverValidX = true;
            //tileAtlasPos.x = (hoverPos.x / tileSetTexture.width) * tilesWidth * tileSize;
            tileWorldPos.x = floor(hoverPos.x / tileSize) * tileSize;
            tileAtlasPos.x = floor(hoverPos.x / tileSize);
        }
        if (hoverPos.y >= 0 && hoverPos.y <= tileSetTexture.height) {
            hoverValidY = true;
            //tileAtlasPos.y = (hoverPos.y / tileSetTexture.height) * tilesHeight * tileSize;
            tileWorldPos.y = floor(hoverPos.y / tileSize) * tileSize;
            tileAtlasPos.y = floor(hoverPos.y / tileSize);
        }
        hoverValidTile = hoverValidX && hoverValidY;
    }

    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    if (openedTileSet) {
        Texture tileSetTexture = tileSet->getTexture();

        DrawTexture(tileSetTexture, 0, 0, WHITE);
        Rectangle border = (Rectangle){
            0, 0,
            (float)tileSet->getTexture().width, (float)tileSet->getTexture().height
        };
        DrawRectangleLinesEx(border, 0.5f, RED);

        //draw tile rects
        int tileSize = tileSet->getTileSize();
        int tilesWidth = tileSetTexture.width / tileSet->getTileSize();
        int tilesHeight = tileSetTexture.height / tileSet->getTileSize();

        for (int y = 0; y < tilesHeight; y++) {
            for (int x = 0; x < tilesWidth; x++){
                Rectangle tileBorder = (Rectangle){
                    tileSize * (float)x, tileSize * (float)y,
                    (float)tileSize, (float)tileSize
                };

                DrawRectangleLinesEx(tileBorder, 0.25f, RED);
            }
        }

        if (hoverValidTile) {
            Rectangle hoverTileRect = (Rectangle){
                tileWorldPos.x, tileWorldPos.y,
                (float)tileSize, (float)tileSize
            };

            DrawRectangleRec(hoverTileRect, Fade(RED, 0.5f));
        }
    }

    EndMode2D();

    DrawCircleV(mousePos, 4, DARKGRAY);
    Vector2 textPos = Vector2Add(mousePos, (Vector2){ -44, -24 });
    int mouseX = hoverPos.x;
    int mouseY = hoverPos.y;
    DrawTextEx(uiFont, TextFormat("[%d, %d]", mouseX, mouseY), textPos, 16, 2, MAROON);

    //draw atlas position text..
    if (hoverValidTile) {
        Vector2 atlasPosTextPos = (Vector2){ 8, static_cast<float>(renderTexture.texture.height - 24) };
        int atlasPosX = tileAtlasPos.x;
        int atlasPosY = tileAtlasPos.y;
        DrawTextEx(uiFont, TextFormat("Tile: [%d, %d]", atlasPosX, atlasPosY), atlasPosTextPos, 16, 2, BLACK);
    }

    EndTextureMode();

    std::string windowTitle = "TileSet not loaded";
    if (openedTileSet) {
        windowTitle = tilePath;
    }

    GuiWindowBox((Rectangle){ 136, 8, 498, 446 }, windowTitle.c_str());

    if (GuiButton((Rectangle){ 8, 8, 120, 24 }, "Open..")) {
        nfdu8char_t *outPath;
        nfdu8filteritem_t filters[1] = { { "RPG++ JSON Resource", "json" } };
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY) {
            if (tileSet != nullptr) {
                tileSet->unload();
                delete tileSet;
            }

            tilePath = outPath;
            tileSet = new TileSet(outPath);
            openedTileSet = true;
        }
    }

    Rectangle cameraRect = (Rectangle){
        0, 0,
        (float)renderTexture.texture.width, -(float)renderTexture.texture.height
    };
    DrawTextureRec(renderTexture.texture, cameraRect, (Vector2){ 138, 32 }, WHITE);
}

void EditorInterfaceService::unload()
{
    if (openedTileSet && tileSet != nullptr) {
        tileSet->unload();
        delete tileSet;
    }
    UnloadRenderTexture(renderTexture);
    UnloadFont(uiFont);

    NFD_Quit();
}

