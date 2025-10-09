#include "editorInterfaceService.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "interactable.hpp"
#include "room.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "worldViewBox.hpp"
#include "tile.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

MapViewBox::MapViewBox() {}

MapViewBox::MapViewBox(WorldViewBox *viewBox, ViewBoxLayer boxLayer)
{
    this->room = nullptr;

    this->viewBox = viewBox;
    this->layer = boxLayer;

    this->tileAtlasPos = Vector2 { 0, 0 };
    this->tileWorldPos = Vector2 { 0, 0 };
    this->hoverValidTile = false;
    this->selectedWorldTile = Vector2 { -1, -1 };

    this->action = ACTION_NONE;
    this->currentLayer = LAYER_TILES;
    this->currentInteractableType = INT_BLANK;
    this->isTileValid = false;
    this->selectedTileAtlasCoords = Vector2 { 0, 0 };
}

void MapViewBox::setMap(Room* map)
{
    this->room = map;
    viewBox->windowTitle = "Map View";
}

void MapViewBox::setActionMode(RoomAction mode)
{
    this->action = mode;
}

void MapViewBox::setLayerMode(RoomLayer mode)
{
    this->currentLayer = mode;
}

void MapViewBox::setCurrentInteractableType(InteractableType type)
{
    this->currentInteractableType = type;
}

void MapViewBox::setSelectedTile(Vector2 tile)
{
    if (tile.x < 0 || tile.y < 0) {
        isTileValid = false;
    } else {
        isTileValid = true;
        selectedTileAtlasCoords = tile;
    }
}

Vector2 MapViewBox::getSelectedWorldTile()
{
    return this->selectedWorldTile;
}

void MapViewBox::isHoverOnValidTile()
{
    if (room == nullptr) return;

    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

    bool hoverValidX = false;
    bool hoverValidY = false;

    TileMap *tileMap = room->getTileMap();
    if (tileMap == nullptr) return;
    int tileSize = tileMap->getAtlasTileSize();

    Vector2 sizeInTiles = tileMap->getMaxWorldSize();
    int absoluteWidth = tileMap->getAtlasTileSize() * sizeInTiles.x;
    int absoluteHeight = tileMap->getAtlasTileSize() * sizeInTiles.y;

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

    Rectangle rect = Rectangle {
        0, 0,
        (tileMap->getMaxWorldSize().x * tileMap->getAtlasTileSize()), (tileMap->getMaxWorldSize().y * tileMap->getAtlasTileSize())
    };

    if (ui.getMouseBoxLayer() != this->layer) return;

    bool isInViewport = viewBox->mouseInput->isInRect();
    bool isInMapRect = CheckCollisionPointRec(viewBox->mouseInput->getMouseWorldPos(), rect);
    if (isInViewport && isInMapRect) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (action == ACTION_PLACE) {
                switch (currentLayer) {
                    case LAYER_TILES:
                        if (isTileValid) {
                            tileMap->setTile(tileAtlasPos, selectedTileAtlasCoords);
                        }
                        break;
                    case LAYER_COLLISIONS:
                        room->getCollisions().addCollisionTile(tileAtlasPos.x, tileAtlasPos.y);
                        break;
                    case LAYER_INTERACTABLES:
                        room->getInteractables().add(tileAtlasPos.x, tileAtlasPos.y, currentInteractableType);
                        break;
                }
            } else if (action == ACTION_ERASE) {
                switch (currentLayer) {
                    case LAYER_TILES:
                        tileMap->setEmptyTile(tileAtlasPos);
                        break;
                    case LAYER_COLLISIONS:
                        room->getCollisions().removeCollisionTile(tileAtlasPos.x, tileAtlasPos.y);
                        break;
                    case LAYER_INTERACTABLES:
                        room->getInteractables().removeInteractable(tileAtlasPos.x, tileAtlasPos.y);
                        break;
                }
            } else if (action == ACTION_EDIT) {
                this->selectedWorldTile = tileAtlasPos;
            }
        }
    }

    if (hoverValidTile) {
        if (currentLayer == LAYER_INTERACTABLES) {
            Interactable* inter = room->getInteractables().get(static_cast<int>(tileAtlasPos.x), static_cast<int>(tileAtlasPos.y));
            if (inter != nullptr) {
                ImGui::SetTooltip("Interactable\nType: %s", Interactable::getTypeNames()[inter->getType()].c_str());
            }
        }
    }
}

void MapViewBox::drawGrid()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen() || room != nullptr) {
        TileMap *tileMap = room->getTileMap();
        if (tileMap == nullptr) return;
        int tileSize = tileMap->getAtlasTileSize();
        //draw a big white rectangle instead
        Rectangle rect = Rectangle {
            0, 0,
            (tileMap->getMaxWorldSize().x * tileSize), (tileMap->getMaxWorldSize().y * tileSize)
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
    TileMap *tileMap = room->getTileMap();
    if (tileMap == nullptr) return;

    int atlasTileSize = tileMap->getAtlasTileSize();
    Vector2 sizeInTiles = tileMap->getMaxWorldSize();
    for (int x = 0; x < sizeInTiles.x; x++) {
        for (int y = 0; y < sizeInTiles.y; y++) {
            Tile tile = tileMap->getTile(x, y);

            if (tile.isPlaced()) {
                //defaults..
                const Vector2 origin = Vector2 { 0.0f, 0.0f };
                const float rotation = 0.0f;

                //texture..
                TileSet* tileSet = tileMap->getTileSet();
                Texture texture = tileMap->getTileSet()->getTexture();

                //actual coordinates
                Vector2 atlasCoords = tileMap->getTile(x, y).getAtlasTile().getAtlasCoords();
                Vector2 worldCoords = Vector2 {
                    static_cast<float>(x * tileMap->getAtlasTileSize()),
                    static_cast<float>(y * tileMap->getAtlasTileSize())
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
                    static_cast<float>(x * tileMap->getAtlasTileSize()),
                    static_cast<float>(y * tileMap->getAtlasTileSize())
                };
                Rectangle worldCoordsRect = Rectangle { worldCoords.x, worldCoords.y, static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize) };

                DrawRectangleLinesEx(worldCoordsRect, 0.25, RED);
            }
        }
    }

    if (currentLayer == LAYER_COLLISIONS) {
        for (auto c : room->getCollisionTiles()) {
            Rectangle collisionRect = Rectangle {
                (c.x * atlasTileSize), (c.y * atlasTileSize),
                static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
            };
            DrawRectangleRec(collisionRect, Fade(RED, 0.8f));
        }
    }
    if (currentLayer == LAYER_INTERACTABLES) {
        EditorInterfaceService& ui = Editor::getUi();
        Font font = ui.getFont();

        for (auto i : room->getInteractableTiles()) {
            Rectangle intRect = Rectangle
            {
                (i->getWorldPos().x * atlasTileSize), (i->getWorldPos().y * atlasTileSize),
                static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
            };

            DrawRectangleRec(intRect, Fade(YELLOW, 0.8f));
            DrawRectangleLinesEx(intRect, 0.5f, YELLOW);

            int typeNum = static_cast<int>(i->getType());
            Vector2 intPos = Vector2 { intRect.x, intRect.y };
            //DrawTextEx(font, TextFormat("Type: %i", typeNum), intPos, 6, 0.5f, BLACK);

            bool validTexture = false;
            Texture2D textureIntType;
            switch (i->getType()) {
                case INT_BLANK:
                    validTexture = true;
                    textureIntType = ui.getBlankTexture();
                    break;
                case INT_TWO:
                    validTexture = true;
                    textureIntType = ui.getDialogTexture();
                    break;
                default:
                    break;
            }
            if (validTexture) {
                DrawTexturePro(textureIntType,
                    Rectangle { 0, 0, static_cast<float>(textureIntType.width), static_cast<float>(textureIntType.height) },
                    intRect,
                    Vector2 { 0, 0 }, 0.0f, WHITE);
            }
        }
    }

    if (action == ACTION_EDIT) {
        if (selectedWorldTile.x > -1) {
            Rectangle hoverTileRect = Rectangle {
                selectedWorldTile.x * atlasTileSize, selectedWorldTile.y * atlasTileSize,
                static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
            };

            DrawRectangleLinesEx(hoverTileRect, 1.0f, Fade(ORANGE, 0.7f));
        }
    }

    drawHoverTile(atlasTileSize, tileWorldPos);
}

void MapViewBox::drawHoverTile(int atlasTileSize, Vector2 tileWorldPos)
{
    TileMap *tileMap = room->getTileMap();
    if (tileMap == nullptr) return;

    if (hoverValidTile) {
        if (action == ACTION_ERASE) {
            Rectangle hoverTileRect = Rectangle {
                tileWorldPos.x, tileWorldPos.y,
                static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
            };

            DrawRectangleRec(hoverTileRect, Fade(GRAY, 0.5f));
        } else if (action == ACTION_PLACE) {
                if (currentLayer == LAYER_TILES && isTileValid) {
                    //defaults
                    Vector2 origin = Vector2 { 0, 0 };
                    float rotation = 0.0f;
                    Color rectColor = Fade(WHITE, 0.7f);

                    //texture..
                    Texture rectTexture = tileMap->getTileSet()->getTexture();

                    //build rects
                    Rectangle selectedTileRect = Rectangle {
                        (selectedTileAtlasCoords.x * atlasTileSize), (selectedTileAtlasCoords.y * atlasTileSize),
                        static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
                    };
                    Rectangle hoverTileRect = Rectangle {
                        tileWorldPos.x, tileWorldPos.y,
                        static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
                    };

                    //draw it
                    DrawTexturePro(rectTexture, selectedTileRect, hoverTileRect, origin, rotation, rectColor);
                } else if (currentLayer == LAYER_COLLISIONS) {
                    Rectangle hoverTileRect = Rectangle {
                        tileWorldPos.x, tileWorldPos.y,
                        static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
                    };

                    DrawRectangleRec(hoverTileRect, Fade(RED, 0.7f));
                } else if (currentLayer == LAYER_INTERACTABLES) {
                    Rectangle hoverTileRect = Rectangle {
                        tileWorldPos.x, tileWorldPos.y,
                        static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
                    };

                    DrawRectangleRec(hoverTileRect, Fade(YELLOW, 0.7f));
                }
        } else if (action == ACTION_EDIT) {
            Rectangle hoverTileRect = Rectangle {
                tileWorldPos.x, tileWorldPos.y,
                static_cast<float>(atlasTileSize), static_cast<float>(atlasTileSize)
            };

            DrawRectangleLinesEx(hoverTileRect, 1.0f, Fade(ORANGE, 0.5f));
        }
    }
}

void MapViewBox::drawMouse()
{
    EditorInterfaceService& ui = Editor::getUi();

    if (ui.getMouseBoxLayer() != this->layer) return;

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

    if (action == ACTION_EDIT && selectedWorldTile.x > -1) {
        Vector2 selectedTileTextPos = Vector2 { 8, 8 };
        int selectedTileWorldPosX = selectedWorldTile.x;
        int selectedTileWorldPosY = selectedWorldTile.y;
        DrawTextEx(ui.getFont(), TextFormat("Selected Tile: [%d, %d]", selectedTileWorldPosX, selectedTileWorldPosY), selectedTileTextPos, 26, 2, ORANGE);
    }
}
