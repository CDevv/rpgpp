#include "worldViewBox.hpp"
#include "tile.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include <rlgl.h>
#include <raymath.h>

MapViewBox::MapViewBox() {}

MapViewBox::MapViewBox(WorldViewBox *viewBox)
{
    this->viewBox = viewBox;
    this->tileAtlasPos = Vector2 { 0, 0 };
    this->tileWorldPos = Vector2 { 0, 0 };
    this->hoverValidTile = false;
}

void MapViewBox::isHoverOnValidTile()
{
    FileSystemService& fs = Editor::getFileSystem();
    bool hoverValidX = false;
    bool hoverValidY = false;

    map = fs.getTileMap();
    int tileSize = map->getAtlasTileSize();
    Vector2 sizeInTiles = map->getMaxWorldSize();
    int absoluteWidth = map->getAtlasTileSize() * sizeInTiles.x;
    int absoluteHeight = map->getAtlasTileSize() * sizeInTiles.y;

    if (viewBox->hoverPos.x >= 0 && viewBox->hoverPos.x <= absoluteWidth) {
        hoverValidX = true;
        tileWorldPos.x = floor(viewBox->hoverPos.x / tileSize) * tileSize;
        tileAtlasPos.x = floor(viewBox->hoverPos.x / tileSize);
    }
    if (viewBox->hoverPos.y >= 0 && viewBox->hoverPos.y <= absoluteHeight) {
        hoverValidY = true;
        tileWorldPos.y = floor(viewBox->hoverPos.y / tileSize) * tileSize;
        tileAtlasPos.y = floor(viewBox->hoverPos.y / tileSize);
    }
    hoverValidTile = hoverValidX && hoverValidY;
}

void MapViewBox::drawGrid()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {
        //rlTranslatef(0, (map->getMaxWorldSize().y)*(map->getAtlasTileSize()), 0);
        //rlRotatef(90, 1, 0, 0);
        //DrawGrid((map->getMaxWorldSize().x), (map->getAtlasTileSize()));

        //draw a big white rectangle instead
        Rectangle rect = Rectangle {
            0, 0,
            (map->getMaxWorldSize().x * map->getAtlasTileSize()), (map->getMaxWorldSize().y * map->getAtlasTileSize())
        };

        DrawRectangleRec(rect, RAYWHITE);
    } else {
        rlTranslatef(0, 25*16, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 16);
    }
}

void MapViewBox::drawTiles()
{
    int atlasTileSize = map->getAtlasTileSize();
    Vector2 sizeInTiles = map->getMaxWorldSize();
    for (int x = 0; x < sizeInTiles.x; x++) {
        for (int y = 0; y < sizeInTiles.y; y++) {
            Tile tile = map->getTile(x, y);

            if (tile.isPlaced()) {
                //defaults..
                const Vector2 origin = Vector2 { 0.0f, 0.0f };
                const float rotation = 0.0f;

                //texture..
                Texture texture = map->getTileSet().getTexture();

                //actual coordinates
                Vector2 atlasCoords = map->getTile(x, y).getAtlasTile().getAtlasCoords();
                Vector2 worldCoords = Vector2 {
                    static_cast<float>(x * map->getAtlasTileSize()),
                    static_cast<float>(y * map->getAtlasTileSize())
                };

                //Build rects
                Rectangle atlasCoordsRect = Rectangle { atlasCoords.x, atlasCoords.y, static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize) };
                Rectangle worldCoordsRect = Rectangle { worldCoords.x, worldCoords.y, static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize) };

                //draw it..
                DrawTexturePro(texture, atlasCoordsRect, worldCoordsRect, origin, rotation, WHITE);

                //draw border
                DrawRectangleLinesEx(worldCoordsRect, 0.25, RED);
            } else {
                //draw empty tile

                Vector2 worldCoords = Vector2 {
                    static_cast<float>(x * map->getAtlasTileSize()),
                    static_cast<float>(y * map->getAtlasTileSize())
                };
                Rectangle worldCoordsRect = Rectangle { worldCoords.x, worldCoords.y, static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize) };

                DrawRectangleLinesEx(worldCoordsRect, 0.25, RED);
            }
        }
    }

    if (hoverValidTile) {
        Rectangle hoverTileRect = Rectangle {
            tileWorldPos.x, tileWorldPos.y,
            static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
        };

        DrawRectangleRec(hoverTileRect, Fade(RED, 0.5f));
    }
}

void MapViewBox::drawMouse()
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

