#include "resourceViewerBox.hpp"
#include <raygui.h>
#include <raylib.h>
#include <string>
#include <vector>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "windowContainer.hpp"

ResourceViewerBox::ResourceViewerBox() {}

ResourceViewerBox::ResourceViewerBox(Rectangle rect)
{
    this->rect = rect;
    this->dropdownEditMode = false;
    this->dropdownActive = 0;
}

void ResourceViewerBox::update() {}

void ResourceViewerBox::draw()
{
    if (dropdownEditMode) GuiLock();

    FileSystemService& fs = Editor::getFileSystem();
    if (fs.getProject() != nullptr) {
        GuiPanel(rect, "Project");

        if (dropdownActive == 0) {
            drawTileSets();
        } else {
            drawMaps();
        }

        if (GuiDropdownBox(Rectangle { rect.x, rect.y + 24, rect.width, 24 }, "TileSets;Rooms", &dropdownActive, dropdownEditMode)) {
            dropdownEditMode = !dropdownEditMode;
        }
    }
}

void ResourceViewerBox::drawTileSets()
{
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New")) {
        windows.openTileSetInit();
    }

    Rectangle baseRect = Rectangle { rect.x, rect.y, rect.width, rect.height };
    baseRect.y += 3*24;
    baseRect.height = 24;

    FileSystemService& fs = Editor::getFileSystem();

    std::vector<std::string> tileSetPaths = fs.getProject()->getTileSetPaths();
    for (std::string tileSetPath : tileSetPaths) {
        std::string tileSetFileName = GetFileNameWithoutExt(tileSetPath.c_str());
        if (GuiLabelButton(baseRect, tileSetFileName.c_str())) {
            fs.openProjectFile(tileSetPath);
            ui.setInitial();
            ui.getTabList().addItem(tileSetFileName);
        }
        baseRect.y += 24;
    }
}

void ResourceViewerBox::drawMaps()
{
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New..")) {
        windows.openMapInit();
    }

    Rectangle baseRect = Rectangle { rect.x, rect.y, rect.width, rect.height };
    baseRect.y += 3*24;
    baseRect.height = 24;

    FileSystemService& fs = Editor::getFileSystem();
    std::vector<std::string> mapPaths = fs.getProject()->getMapPaths();
    for (std::string mapPath : mapPaths) {
        std::string mapFileName = GetFileNameWithoutExt(mapPath.c_str());
        if (GuiLabelButton(baseRect, mapFileName.c_str())) {
            fs.openProjectFile(mapPath);
            ui.setInitial();
            ui.getTabList().addItem(mapFileName);
        }
        baseRect.y += 24;
    }
}