#include "worldViewBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "room.hpp"
#include "tileset.hpp"
#include <memory>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

WorldViewBox::WorldViewBox()
{}

WorldViewBox::WorldViewBox(Rectangle windowRect, Rectangle renderRect, EngineFileType type)
{
    windowTitle = "File not opened..";
    this->type = type;

    camera = Camera2D { {0} };
    camera.target = Vector2 { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    this->mousePos = Vector2 { 0, 0 };
    this->hoverPos = Vector2 { 0, 0 };

    this->windowRect = windowRect;
    this->renderRect = renderRect;

    renderTexture = LoadRenderTexture(renderRect.width, renderRect.height);

    mouseInput = std::make_unique<MouseInputComponent>(Vector2 { renderRect.x, renderRect.y }, camera, renderRect);

    tilesView = TileSetViewBox(this);
    mapView = MapViewBox(this);
}

WorldViewBox::~WorldViewBox()
{
    UnloadRenderTexture(renderTexture);
}

void WorldViewBox::setTileSet(TileSet* tileSet)
{
    tilesView.setTileSet(tileSet);
}

void WorldViewBox::setRoom(Room* map)
{
    mapView.setMap(map);
}

void WorldViewBox::enableTileSelection()
{
    if (type == FILE_TILESET) {
        tilesView.setSelectionMode();
    }
}

Vector2 WorldViewBox::getSelectedTile()
{
    return tilesView.getSelectedTile();
}

void WorldViewBox::enableTilePlacement()
{
    mapView.setPlacingMode();
}

void WorldViewBox::setSelectedTile(Vector2 tile)
{
    mapView.setSelectedTile(tile);
}

void WorldViewBox::update()
{
    mouseInput->update();
    mousePos = mouseInput->getMousePos();
    hoverPos = mouseInput->getMouseWorldPos();
}

void WorldViewBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {
        if (type == FILE_TILESET) {
            tilesView.isHoverOnValidTile();
        } else {
            mapView.isHoverOnValidTile();
        }
    }

    BeginTextureMode(renderTexture);
    if (type == FILE_ROOM) {
        ClearBackground(GRAY);
    } else {
        ClearBackground(RAYWHITE);
    }
    BeginMode2D(camera);

    rlPushMatrix();
    if (type == FILE_TILESET) {
        tilesView.drawGrid();
    } else {
        mapView.drawGrid();
    }
    rlPopMatrix();

    if (fs.fileIsOpen()) {
        if (type == FILE_TILESET) {
            tilesView.drawTiles();
        } else {
            mapView.drawTiles();
        }
    }

    EndMode2D();

    if (type == FILE_TILESET) {
        tilesView.drawMouse();
    } else {
        mapView.drawMouse();
    }

    EndTextureMode();

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

