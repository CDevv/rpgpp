#include <raygui.h>
#include <raymath.h>
#include <rlgl.h>
#include "nfd.h"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include "editor.hpp"

EditorInterfaceService::EditorInterfaceService()
{
    //get codepoints
    std::vector<int> codepoints;
    //ASCII latin
    for (int i = 0; i < 92; i++) {
        codepoints.push_back(i + 32);
    }
    //cyrillic
    for (int i = 0x400; i <= 0x4FF; i++) {
        codepoints.push_back(i);
    }

    //load the font
    uiFont = LoadFontEx("resources/LanaPixel.ttf", 13, codepoints.data(), codepoints.size());

    GuiLoadStyle("rpgpp.rgs");
    GuiSetFont(uiFont);

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

    Rectangle propRect = Rectangle { 8, 48, 160, static_cast<float>(GetScreenHeight() - 56) };
    propertiesBox = PropertiesBox(propRect);

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
    propertiesBox.update();
}

void EditorInterfaceService::draw()
{
    worldView->draw();

    FileSystemService& fs = Editor::getFileSystem();
    if (GuiButton(Rectangle { 8, 8, 120, 24 }, "Open..")) {
        fs.promptOpenFile();

        propertiesBox.setDefaults();
    }

    if (fs.fileIsOpen()) {
        if (fs.getType() == FILE_TILESET) {
            TileSet *tileSet = fs.getTileSet();

            if (GuiButton(Rectangle { 138, 8, 120, 24 }, "Save")) {
                std::string jsonString = tileSet->dumpJson().dump(4);

                char *text = const_cast<char*>(jsonString.data());
                SaveFileText(fs.getOpenedFilePath().c_str(), text);
            }
        }

        propertiesBox.draw();
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

void EditorInterfaceService::drawTooltip(Rectangle rect, std::string text)
{
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        Vector2 mousePos = Vector2Add(GetMousePosition(), Vector2 { 16, 0 });
        Vector2 textPos = Vector2Add(mousePos, Vector2 { 4, 4 });
        Vector2 textSize = MeasureTextEx(uiFont, text.c_str(), 13, 2);
        GuiPanel(
            Rectangle {
                mousePos.x, mousePos.y,
                textSize.x + 8, textSize.y + 8
            }, NULL);
        DrawTextEx(uiFont, text.c_str(), textPos, 13, 2, GRAY);
    }
}


