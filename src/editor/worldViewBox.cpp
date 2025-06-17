#include "worldViewBox.hpp"
#include "editor.hpp"
#include <raygui.h>
#include <raymath.h>
#include <rlgl.h>
#include <cmath>

WorldViewBox::WorldViewBox()
{}

WorldViewBox::WorldViewBox(Rectangle windowRect, Rectangle renderRect)
{
    windowTitle = "TileSet not opened..";

    camera = Camera2D { {0} };
    camera.target = Vector2 { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    this->windowRect = windowRect;
    this->renderRect = renderRect;

    renderTexture = LoadRenderTexture(renderRect.width, renderRect.height);

    mouseInput = std::make_unique<MouseInputComponent>(Vector2 { renderRect.x, renderRect.y }, camera, renderRect);
}

void WorldViewBox::update()
{
    mouseInput->update();
    mousePos = mouseInput->getMousePos();
    hoverPos = mouseInput->getMouseWorldPos();
}

void WorldViewBox::draw()
{
    EditorInterfaceService& ui = Editor::getUi();
    FileSystemService& fs = Editor::getFileSystem();

    //Vector2 mousePos = mouseInput->getMousePos();
    //Vector2 hoverPos = mouseInput->getMouseWorldPos();

    Vector2 tileAtlasPos = Vector2 { 0, 0 };
    Vector2 tileWorldPos = Vector2 { 0, 0 };
    bool hoverValidX = false;
    bool hoverValidY = false;
    bool hoverValidTile = false;

    if (fs.fileIsOpen()) {
        tileSet = fs.getTileSet();

        Texture tileSetTexture = tileSet->getTexture();
        int tileSize = tileSet->getTileSize();

        if (hoverPos.x >= 0 && hoverPos.x <= tileSetTexture.width) {
            hoverValidX = true;
            tileWorldPos.x = floor(hoverPos.x / tileSize) * tileSize;
            tileAtlasPos.x = floor(hoverPos.x / tileSize);
        }
        if (hoverPos.y >= 0 && hoverPos.y <= tileSetTexture.height) {
            hoverValidY = true;
            tileWorldPos.y = floor(hoverPos.y / tileSize) * tileSize;
            tileAtlasPos.y = floor(hoverPos.y / tileSize);
        }
        hoverValidTile = hoverValidX && hoverValidY;
    }

    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    rlPushMatrix();

    if (fs.fileIsOpen()) {
        rlTranslatef(0, 25*(tileSet->getTileSize()), 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, (tileSet->getTileSize()));
    } else {
        rlTranslatef(0, 25*16, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 16);
    }

    rlPopMatrix();

    if (fs.fileIsOpen()) {
        Texture tileSetTexture = tileSet->getTexture();

        DrawTexture(tileSetTexture, 0, 0, WHITE);
        Rectangle border = Rectangle {
            0, 0,
            static_cast<float>(tileSet->getTexture().width), static_cast<float>(tileSet->getTexture().height)
        };
        DrawRectangleLinesEx(border, 0.5f, RED);

        //draw tile rects
        int tileSize = tileSet->getTileSize();
        int tilesWidth = tileSetTexture.width / tileSet->getTileSize();
        int tilesHeight = tileSetTexture.height / tileSet->getTileSize();

        for (int y = 0; y < tilesHeight; y++) {
            for (int x = 0; x < tilesWidth; x++){
                Rectangle tileBorder = Rectangle {
                    tileSize * (float)x, tileSize * (float)y,
                    (float)tileSize, (float)tileSize
                };

                DrawRectangleLinesEx(tileBorder, 0.25f, RED);
            }
        }

        if (hoverValidTile) {
            Rectangle hoverTileRect = Rectangle {
                tileWorldPos.x, tileWorldPos.y,
                static_cast<float>(tileSize), static_cast<float>(tileSize)
            };

            DrawRectangleRec(hoverTileRect, Fade(RED, 0.5f));
        }
    }

    EndMode2D();

    if (mouseInput->isInRect()) {
        //small circle on mouse pos
        DrawCircleV(mousePos, 4, DARKGRAY);
        Vector2 textPos = Vector2Add(mousePos, Vector2 { -44, -24 });
        int mouseX = hoverPos.x;
        int mouseY = hoverPos.y;
        DrawTextEx(ui.getFont(), TextFormat("[%d, %d]", mouseX, mouseY), textPos, 16, 2, MAROON);

        //draw atlas position text..
        if (hoverValidTile) {
            Vector2 atlasPosTextPos = Vector2 { 8, static_cast<float>(renderTexture.texture.height - 24) };
            int atlasPosX = tileAtlasPos.x;
            int atlasPosY = tileAtlasPos.y;
            DrawTextEx(ui.getFont(), TextFormat("Tile: [%d, %d]", atlasPosX, atlasPosY), atlasPosTextPos, 16, 2, BLACK);
        }
    }

    EndTextureMode();

    std::string windowTitle = "TileSet not loaded";
    if (fs.fileIsOpen()) {
        windowTitle = fs.getOpenedFilePath();
    }
    GuiWindowBox(windowRect, windowTitle.c_str());

    Rectangle cameraRect = Rectangle {
        0, 0,
        static_cast<float>(renderTexture.texture.width), static_cast<float>(-renderTexture.texture.height)
    };
    DrawTextureRec(renderTexture.texture, cameraRect, Vector2 { renderRect.x, renderRect.y }, WHITE);
}

void WorldViewBox::unload()
{
    UnloadRenderTexture(renderTexture);
}

