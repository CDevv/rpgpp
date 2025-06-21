#include "worldViewBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include <rlgl.h>
#include <raymath.h>

TileSetViewBox::TileSetViewBox() {}

TileSetViewBox::TileSetViewBox(WorldViewBox* viewBox)
{
    this->updateSelf = true;

    this->viewBox = viewBox;
    this->tileAtlasPos = Vector2 { 0, 0 };
    this->tileWorldPos = Vector2 { 0, 0 };
    this->hoverValidTile = false;
}

TileSetViewBox::TileSetViewBox(WorldViewBox* viewBox, TileSet *tileSet)
{
    this->updateSelf = false;

    this->viewBox = viewBox;
    this->tileAtlasPos = Vector2 { 0, 0 };
    this->tileWorldPos = Vector2 { 0, 0 };
    this->hoverValidTile = false;

    this->tileSet = tileSet;
}

void TileSetViewBox::isHoverOnValidTile()
{
    FileSystemService& fs = Editor::getFileSystem();
    bool hoverValidX = false;
    bool hoverValidY = false;

    if (updateSelf) {
        this->tileSet = fs.getTileSet();
    }

    Texture tileSetTexture = tileSet->getTexture();
    int tileSize = tileSet->getTileSize();

    if (viewBox->hoverPos.x >= 0 && viewBox->hoverPos.x <= tileSetTexture.width) {
        hoverValidX = true;
        tileWorldPos.x = floor(viewBox->hoverPos.x / tileSize) * tileSize;
        tileAtlasPos.x = floor(viewBox->hoverPos.x / tileSize);
    }
    if (viewBox->hoverPos.y >= 0 && viewBox->hoverPos.y <= tileSetTexture.height) {
        hoverValidY = true;
        tileWorldPos.y = floor(viewBox->hoverPos.y / tileSize) * tileSize;
        tileAtlasPos.y = floor(viewBox->hoverPos.y / tileSize);
    }
    hoverValidTile = hoverValidX && hoverValidY;
}

void TileSetViewBox::drawGrid()
{
    FileSystemService& fs = Editor::getFileSystem();
    if (fs.fileIsOpen()) {
        rlTranslatef(0, 25*(tileSet->getTileSize()), 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, (tileSet->getTileSize()));
    } else {
        rlTranslatef(0, 25*16, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 16);
    }
}

void TileSetViewBox::drawTiles()
{
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

void TileSetViewBox::drawMouse()
{
    EditorInterfaceService& ui = Editor::getUi();

    if (viewBox->mouseInput->isInRect()) {
        //small circle on mouse pos
        DrawCircleV(viewBox->mousePos, 4, DARKGRAY);
        Vector2 textPos = Vector2Add(viewBox->mousePos, Vector2 { -44, -24 });
        int mouseX = viewBox->hoverPos.x;
        int mouseY = viewBox->hoverPos.y;
        DrawTextEx(ui.getFont(), TextFormat("[%d, %d]", mouseX, mouseY), textPos, 26, 2, MAROON);

        //draw atlas position text..
        if (hoverValidTile) {
            Vector2 atlasPosTextPos = Vector2 { 8, static_cast<float>(viewBox->renderTexture.texture.height - 24) };
            int atlasPosX = tileAtlasPos.x;
            int atlasPosY = tileAtlasPos.y;
            DrawTextEx(ui.getFont(), TextFormat("Tile: [%d, %d]", atlasPosX, atlasPosY), atlasPosTextPos, 26, 2, BLACK);
        }
    }
}


