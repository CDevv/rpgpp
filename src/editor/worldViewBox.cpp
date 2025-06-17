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

    tilesView = TileSetViewBox(this);
    mapView = MapViewBox(this);
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
        if (fs.getType() == FILE_TILESET) {
            tilesView.isHoverOnValidTile();
        } else {
            mapView.isHoverOnValidTile();
        }
    }

    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    rlPushMatrix();
    if (fs.getType() == FILE_TILESET) {
        tilesView.drawGrid();
    } else {
        mapView.drawGrid();
    }
    rlPopMatrix();

    if (fs.fileIsOpen()) {
        if (fs.getType() == FILE_TILESET) {
            tilesView.drawTiles();
        } else {
            mapView.drawTiles();
        }
    }

    EndMode2D();

    if (fs.getType() == FILE_TILESET) {
        tilesView.drawMouse();
    } else {
        mapView.drawMouse();
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

