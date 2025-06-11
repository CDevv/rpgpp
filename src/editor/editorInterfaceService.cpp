#include "editorInterfaceService.hpp"
#include <raygui.h>
#include <raymath.h>
#include <rlgl.h>
#include "nfd.h"

EditorInterfaceService::EditorInterfaceService()
{
    NFD_Init();

    uiFont = LoadFont("resources/ark-pixel-10px-monospaced-latin.otf");

    openDialog = false;
    openedTileSet = false;
    tileSet = nullptr;
    mousePos = (Vector2){ 0, 0 };
    hoverPos = (Vector2){ 0, 0 };

    camera = (Camera2D){ {0} };
    camera.target = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    renderTexture = LoadRenderTexture(494, 420);

    mouseInput = new MouseInputComponent((Vector2){ 138, 32 }, &camera);
    worldView = WorldViewBox(&camera);

    GuiLoadStyle("rpgpp.rgs");
}

void EditorInterfaceService::update()
{
    mouseInput->update();

    mousePos = mouseInput->getMousePos();
    hoverPos = mouseInput->getMouseWorldPos();
}

void EditorInterfaceService::draw()
{
    worldView.draw();

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
}

void EditorInterfaceService::unload()
{
    if (openedTileSet && tileSet != nullptr) {
        tileSet->unload();
        delete tileSet;
    }
    UnloadFont(uiFont);
    worldView.unload();

    NFD_Quit();
}

std::string EditorInterfaceService::getTilePath()
{
    return tilePath;
}

bool EditorInterfaceService::hasOpenedTileSet()
{
    return openedTileSet;
}

TileSet * EditorInterfaceService::getTileSet()
{
    return tileSet;
}

MouseInputComponent *EditorInterfaceService::getMouse()
{
    return mouseInput;
}

Font EditorInterfaceService::getFont()
{
    return uiFont;
}

Camera2D *EditorInterfaceService::getCamera()
{
    return &camera;
}

