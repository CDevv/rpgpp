#include "worldViewBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "room.hpp"
#include "tileset.hpp"
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

WorldViewBox::WorldViewBox()
{}

WorldViewBox::WorldViewBox(Rectangle windowRect, EngineFileType type, ViewBoxLayer boxLayer)
{
    windowTitle = "WorldViewBox";
    this->type = type;
    this->isChild = false;

    camera = Camera2D { {0} };
    camera.target = Vector2 { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    this->mousePos = Vector2 { 0, 0 };
    this->hoverPos = Vector2 { 0, 0 };

    this->windowRect = windowRect;
    this->renderRect = Rectangle
    {
        (windowRect.x + 2), (windowRect.y + 24),
        (windowRect.width - 4), (windowRect.height - 30)
    };

    renderTexture = LoadRenderTexture(this->renderRect.width, this->renderRect.height);

    mouseInput = std::make_unique<MouseInputComponent>(Vector2 { this->renderRect.x, this->renderRect.y },
        camera, this->renderRect, static_cast<int>(boxLayer));

    tilesView = TileSetViewBox(this, boxLayer);
    mapView = MapViewBox(this, boxLayer);

    this->mouseLock = false;
}

WorldViewBox::~WorldViewBox()
{
    UnloadRenderTexture(renderTexture);
}

void WorldViewBox::setWindowTitle(std::string windowTitle)
{
    this->windowTitle = windowTitle;
}

void WorldViewBox::asChild()
{
    this->isChild = true;
}

Rectangle WorldViewBox::getWindowRect()
{
    return this->windowRect;
}

Rectangle WorldViewBox::getRenderRect()
{
    Rectangle rec = windowRect;
    rec.y += 21;
    rec.height -= 21;
    return rec;
}

void WorldViewBox::setRect(Rectangle rect)
{
    this->windowRect = rect;
}

void WorldViewBox::setMouseLock(bool value)
{
    this->mouseLock = value;
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

void WorldViewBox::setActionMode(RoomAction mode)
{
    mapView.setActionMode(mode);
}

void WorldViewBox::setLayerMode(RoomLayer mode)
{
    mapView.setLayerMode(mode);
}

void WorldViewBox::setCurrentInteractableType(InteractableType type)
{
    mapView.setCurrentInteractableType(type);
}

void WorldViewBox::setSelectedTile(Vector2 tile)
{
    mapView.setSelectedTile(tile);
}

void WorldViewBox::setStringProp(std::string str)
{
    mapView.setStringProp(str);
}

Vector2 WorldViewBox::getSelectedWorldTile()
{
    return mapView.getSelectedWorldTile();
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

    if (type == FILE_TILESET) {
        tilesView.isHoverOnValidTile();
    } else {
        mapView.isHoverOnValidTile();
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

    if (type == FILE_TILESET) {
        tilesView.drawTiles();
    } else {
        mapView.drawTiles();
    }

    EndMode2D();

    if (type == FILE_TILESET) {
        tilesView.drawMouse();
    } else {
        mapView.drawMouse();
    }

    EndTextureMode();


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    bool success = false;
    if (!isChild) {
        ImGui::SetNextWindowPos(ImVec2 { windowRect.x, windowRect.y });
        ImGui::SetNextWindowSize(ImVec2 { windowRect.width, windowRect.height });
        success = ImGui::Begin(windowTitle.c_str(), NULL,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
    } else {
        ImGui::SetNextWindowSize(ImVec2 { windowRect.width, -1 });
        success = ImGui::BeginChild(windowTitle.c_str());
    }

    if (success) {

        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        Rectangle windowRect = Rectangle {
            pos.x, pos.y, size.x, size.y
        };

        Rectangle rec = windowRect;
        rec.y += 21;
        rec.height -= 21;

        this->renderRect = rec;

        if (renderRect.width != renderTexture.texture.width) {
            UnloadRenderTexture(renderTexture);
            renderTexture = LoadRenderTexture(renderRect.width, renderRect.height);
        }

        this->mouseInput->setCameraRect(renderRect);

        rlImGuiImageRenderTexture(&renderTexture);

        if (isChild) {
            ImGui::EndChild();
        } else {
            ImGui::End();
        }
    }
    ImGui::PopStyleVar();
}

void WorldViewBox::unload()
{
    UnloadRenderTexture(renderTexture);
}
