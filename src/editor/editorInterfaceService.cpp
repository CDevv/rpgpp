#include "editorInterfaceService.hpp"
#include <raygui.h>
#include <raymath.h>
#include "nfd.h"

EditorInterfaceService::EditorInterfaceService()
{
    NFD_Init();

    openDialog = false;
    openedTileSet = false;
    tileSet = nullptr;

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
    }

    EndMode2D();
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

    NFD_Quit();
}

