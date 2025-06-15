#include <raygui.h>
#include <raymath.h>
#include <rlgl.h>
#include "nfd.h"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include "editor.hpp"

EditorInterfaceService::EditorInterfaceService()
{
    uiFont = LoadFont("resources/ark-pixel-10px-monospaced-latin.otf");

    mousePos = Vector2 { 0, 0 };
    hoverPos = Vector2 { 0, 0 };

    camera = Camera2D { {0} };
    camera.target = Vector2 { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    mouseInput = std::make_unique<MouseInputComponent>(Vector2 { 138, 32 }, camera);
    worldView = WorldViewBox(&camera);

    GuiLoadStyle("rpgpp.rgs");

    chosenTileSize = 0;
    chosenTileSizeEditMode = false;
}

EditorInterfaceService::~EditorInterfaceService()
{
    UnloadFont(uiFont);
    worldView.unload();
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

    FileSystemService& fs = Editor::getFileSystem();
    if (GuiButton(Rectangle { 8, 8, 120, 24 }, "Open..")) {
        fs.promptOpenFile();

        TileSet *tileSet = fs.getTileSet();
        chosenTileSize = tileSet->getTileSize();
    }

    if (fs.fileIsOpen()) {
        TileSet *tileSet = fs.getTileSet();
        if (chosenTileSize >= 16) {
            tileSet->setTileSize(chosenTileSize);
        }

        GuiLabel(Rectangle { 8, 40, 120, 24 }, "TILE SIZE");
        if (GuiValueBox(Rectangle { 8, 72, 120, 24 }, "", &chosenTileSize, 16, 32, chosenTileSizeEditMode)) {
            chosenTileSizeEditMode = !chosenTileSizeEditMode;
        }

        GuiLabel(Rectangle { 8, 112, 120, 24 }, "TEXTURE SOURCE");
        if (GuiLabelButton(Rectangle { 8, 144, 120, 24 }, tileSet->getTextureSource().c_str())) {
            FS_Result fsResult = fs.openImage();
            if (fsResult.result == NFD_OKAY) {
                tileSet->setTextureSource(fsResult.path);
                printf("%s \n", fsResult.path.c_str());
            }
        }
    }
}

void EditorInterfaceService::unload()
{
    UnloadFont(uiFont);
    
    worldView.unload();
}

MouseInputComponent& EditorInterfaceService::getMouse()
{
    return *mouseInput.get();
}

Font EditorInterfaceService::getFont()
{
    return uiFont;
}

Camera2D& EditorInterfaceService::getCamera()
{
    return camera;
}

