#include "mapInitWindow.hpp"
#include <memory>
#include <raygui.h>
#include <raylib.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "room.hpp"
#include "tilemap.hpp"

MapInitWindow::MapInitWindow() {}

MapInitWindow::MapInitWindow(Rectangle rect)
{
    this->rect = rect;
    this->active = false;

    this->titleText = "";
    this->title = std::make_unique<char>();
    *title = '\0';
    this->titleEditMode = false;
    this->tileSetSource = "";
    this->hasSetTileSet = false;
}

void MapInitWindow::setActive()
{
    active = true;
}

void MapInitWindow::closeWindow()
{
    active = false;

    this->titleText = "";
    this->title = std::make_unique<char>();
    *title = '\0';
    this->titleEditMode = false;
    this->tileSetSource = "";
    this->hasSetTileSet = false;
}

void MapInitWindow::draw()
{
    if (active) {
        FileSystemService& fs = Editor::getFileSystem();
        EditorInterfaceService& ui = Editor::getUi();

        if (GuiWindowBox(rect, "New Room..")) {
            active = !active;
        }

        GuiLabel(Rectangle { rect.x + 8, rect.y + 40, rect.width - 16, 24 }, "Title..");
        if (GuiTextBox(Rectangle { rect.x + 8, rect.y + 64, rect.width - 16, 24 }, title.get(), 13, titleEditMode)) {
            titleEditMode = !titleEditMode;
        }

        GuiLabel(Rectangle { rect.x + 8, rect.y + 96, rect.width - 16, 24 }, "TileSet..");

        Rectangle tileSetSourceLabelRect = Rectangle {
            rect.x + 8, rect.y + 120, rect.width - (16 + 24), 24
        };
        if (hasSetTileSet) {
            GuiLabel(tileSetSourceLabelRect, tileSetSource.c_str());
        } else {
            GuiLabel(tileSetSourceLabelRect, "Not set..");
        }

        if (GuiButton(Rectangle { rect.x + 8 + (rect.width - (16 + 24)), rect.y + 120, 24, 24 }, GuiIconText(ICON_FILE_OPEN, NULL))) {
            FS_Result fsResult = fs.openTileSetResource();
            if (fsResult.result == NFD_OKAY) {
                tileSetSource = fsResult.path;
                hasSetTileSet = true;
            }
        }

        if (GuiButton(Rectangle { rect.x + 184, rect.y + (rect.height - (24 + 8)), 120, 24 }, "Create..")) {
            titleText = title.get();

            if (titleText.empty()) return;
            if (!hasSetTileSet) return;

            std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>(tileSetSource, 25, 25, 16, 48);

            Room room(std::move(tileMap));
            nlohmann::json roomJson = room.dumpJson();
            std::string jsonString = roomJson.dump(4);

            std::string filePath = std::string("maps/").append(titleText).append(".rmap");
            SaveFileText(filePath.c_str(), const_cast<char*>(jsonString.data()));

            fs.openProjectFile(filePath);
            ui.setInitial();

            closeWindow();
        }
    }
}