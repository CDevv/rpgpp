#include "tileSetInitWindow.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "nfd.h"
#include "tileset.hpp"
#include <cstring>
#include <memory>
#include <raygui.h>
#include <raylib.h>

TileSetInitWindow::TileSetInitWindow() {}

TileSetInitWindow::TileSetInitWindow(Rectangle rect)
{
    this->active = false;
    this->rect = rect;

    this->titleEditMode = false;
    this->titleText = "";
    this->hasSetTextureSource = false;

    this->title = std::make_unique<char>();
    *title = '\0';
}

void TileSetInitWindow::setActive()
{
    active = true;
}

void TileSetInitWindow::closeWindow()
{
    hasSetTextureSource = false;
    textureSource = "";
    titleText = "";
    *title = '\0';
    active = false;
}

void TileSetInitWindow::draw()
{
    if (active) {
        FileSystemService& fs = Editor::getFileSystem();
        EditorInterfaceService& ui = Editor::getUi();

        if (GuiWindowBox(rect, "New TileSet..")) {
            active = !active;
        }

        GuiLabel(Rectangle { rect.x + 8, rect.y + 40, rect.width - 16, 24 }, "Title..");
        if (GuiTextBox(Rectangle { rect.x + 8, rect.y + 64, rect.width - 16, 24 }, title.get(), 13, titleEditMode)) {
            titleEditMode = !titleEditMode;
        }

        GuiLabel(Rectangle { rect.x + 8, rect.y + 96, rect.width - 16, 24 }, "Texture..");
        Rectangle textureSourceLabelRect = Rectangle { rect.x + 8, rect.y + 120, rect.width - (16 + 24), 24 };
        if (hasSetTextureSource) {
            GuiLabel(textureSourceLabelRect, textureSource.c_str());
        } else {
            GuiLabel(textureSourceLabelRect, "Not set..");
        }
        if (GuiButton(Rectangle { rect.x + 8 + (rect.width - (16 + 24)), rect.y + 120, 24, 24 }, GuiIconText(ICON_FILE_OPEN, NULL))) {
            FS_Result fsResult = fs.openImage();
            if (fsResult.result == NFD_OKAY) {
                textureSource = fsResult.path;
                hasSetTextureSource = true;
            }
        }

        if (GuiButton(Rectangle { rect.x + 184, rect.y + (rect.height - (24 + 8)), 120, 24 }, "Submit..")) {
            titleText = title.get();
            
            if (titleText.empty()) return;
            if (!hasSetTextureSource) return;

            TileSet tileSet(textureSource, 16);
            nlohmann::json tileSetJson = tileSet.dumpJson();
            std::string jsonString = tileSetJson.dump(4);

            std::string filePath = std::string("tilesets/").append(titleText).append(".rtiles");
            SaveFileText(filePath.c_str(), const_cast<char*>(jsonString.data()));

            fs.openProjectFile(filePath);
            ui.setInitial();
            
            closeWindow();
        }

        if (CheckCollisionPointRec(GetMousePosition(), rect)) {
            ui.setMouseLock(true);
        } else {
            ui.setMouseLock(false);
        }
    } else {
        EditorInterfaceService& ui = Editor::getUi();
        ui.setMouseLock(false);
    }
}