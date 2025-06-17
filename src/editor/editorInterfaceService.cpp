#include <raygui.h>
#include <raymath.h>
#include <rlgl.h>
#include "nfd.h"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include "editor.hpp"

EditorInterfaceService::EditorInterfaceService()
{
    GuiLoadStyle("rpgpp.rgs");
    uiFont = LoadFont("resources/ark-pixel-10px-monospaced-latin.otf");

    mousePos = Vector2 { 0, 0 };
    hoverPos = Vector2 { 0, 0 };


    Rectangle windowRect = Rectangle
    {
        176, 48,
        static_cast<float>(GetScreenWidth() - 386), static_cast<float>(GetScreenHeight() - 56)
    };
    Rectangle renderRect = Rectangle {
        (windowRect.x + 2), (windowRect.y + 24),
        (windowRect.width - 4), (windowRect.height - 30)
    };
    worldView = std::make_unique<WorldViewBox>(windowRect, renderRect);

    chosenTileSize = 0;
    chosenTileSizeEditMode = false;
}

EditorInterfaceService::~EditorInterfaceService()
{
    UnloadFont(uiFont);
    worldView->unload();
}

void EditorInterfaceService::update()
{
    worldView->update();
}

void EditorInterfaceService::draw()
{
    worldView->draw();

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

        if (GuiButton(Rectangle { 138, 8, 120, 24 }, "Save")) {
            std::string jsonString = tileSet->dumpJson().dump(4);

            char *text = jsonString.data();
            SaveFileText(fs.getOpenedFilePath().c_str(), text);
        }

        GuiPanel(Rectangle { 8, 48, 160, static_cast<float>(GetScreenHeight() - 56) }, "TileSet Props");

        GuiLabel(Rectangle { 16, 80, 144, 24 }, "TILE SIZE");
        if (GuiValueBox(Rectangle { 16, 112, 144, 24 }, NULL, &chosenTileSize, 16, 32, chosenTileSizeEditMode)) {
            chosenTileSizeEditMode = !chosenTileSizeEditMode;
        }

        GuiLabel(Rectangle { 16, 144, 144, 24 }, "TEXTURE SOURCE");
        std::string sourceFileName = GetFileName(tileSet->getTextureSource().c_str());
        GuiLabel(Rectangle { 16, 176, 144, 24 }, sourceFileName.c_str());
        if (CheckCollisionPointRec(GetMousePosition(), Rectangle { 16, 176, 144, 24 })) {
            Vector2 mousePos = GetMousePosition();
            Vector2 textPos = Vector2Add(mousePos, Vector2 { 2, 2 });
            Vector2 textSize = MeasureTextEx(uiFont, tileSet->getTextureSource().c_str(), 16, 2);
            GuiPanel(
                Rectangle {
                    mousePos.x, mousePos.y,
                    textSize.x + 4, textSize.y + 4
                }, NULL);
            DrawTextEx(uiFont, tileSet->getTextureSource().c_str(), textPos, 16, 2, GRAY);
        }

        if (GuiButton(Rectangle { 16, 208, 144, 24 }, "CHANGE TEXTURE")) {
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
    worldView->unload();
}

Font EditorInterfaceService::getFont()
{
    return uiFont;
}

