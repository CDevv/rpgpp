#include "mapPanelView.hpp"
#include <memory>
#include <raylib.h>
#include <raygui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "propertiesBox.hpp"
#include "worldViewBox.hpp"

MapPanelView::MapPanelView()
{
}

MapPanelView::MapPanelView(Rectangle rect)
{
    actionModeToggle = 0;
    layoutModeToggle = 0;
    layoutDropdownEditMode = false;

    Rectangle windowRect = Rectangle
    {
        176, 48,
        static_cast<float>(GetScreenWidth() - 464), static_cast<float>(GetScreenHeight() - 56)
    };
    Rectangle renderRect = Rectangle {
        (windowRect.x + 2), (windowRect.y + 24),
        (windowRect.width - 4), (windowRect.height - 30)
    };
    worldView = std::make_unique<WorldViewBox>(windowRect, renderRect, FILE_ROOM);

    Rectangle tileSetWindowRect = Rectangle
    {
        (windowRect.x + windowRect.width + 4), (windowRect.y + 56),
        (GetScreenWidth() - ((windowRect.x + windowRect.width + 4) + 4)), (8)
    };
    tileSetWindowRect.height = tileSetWindowRect.width - 36;
    Rectangle tileSetRenderRect = Rectangle
    {
        (tileSetWindowRect.x + 2), (tileSetWindowRect.y + 24),
        (tileSetWindowRect.width - 4), (tileSetWindowRect.height - 30)
    };
    tileSetView = std::make_unique<WorldViewBox>(tileSetWindowRect, tileSetRenderRect, FILE_TILESET);

    tileSetView->enableTileSelection();
    worldView->setActionMode(ACTION_PLACE);

    Rectangle propRect = Rectangle
    {
        (tileSetWindowRect.x), (tileSetWindowRect.y + tileSetWindowRect.height + 8)
    };
    propRect.width = tileSetWindowRect.width;
    propRect.height = GetScreenHeight() - propRect.y - 8;
    propBox = PropertiesBox(propRect);
}

void MapPanelView::setInitial()
{
    propBox.setDefaults();
}

void MapPanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();

    worldView->setRoom(fs.getRoom());
    worldView->update();

    if (fs.getRoom() != nullptr) {
        tileSetView->setTileSet(&fs.getRoom()->getTileMap()->getTileSet());
    }
    tileSetView->update();

    worldView->setSelectedTile(tileSetView->getSelectedTile());

    propBox.update();

    RoomAction actionMode = static_cast<RoomAction>(actionModeToggle + 1);
    worldView->setActionMode(actionMode);

    RoomLayer layerMode = static_cast<RoomLayer>(layoutModeToggle);
    worldView->setLayerMode(layerMode);
}

void MapPanelView::draw()
{
    if (layoutDropdownEditMode) GuiLock();

    worldView->draw();
    if (layoutModeToggle == 0) {
        tileSetView->draw();
    }
    propBox.draw();

    Rectangle windowRect = Rectangle
    {
        176, 48,
        static_cast<float>(GetScreenWidth() - 464), static_cast<float>(GetScreenHeight() - 56)
    };
    Rectangle optionsRect = Rectangle 
    {
        (windowRect.x + windowRect.width + 4), (windowRect.y),
        (GetScreenWidth() - ((windowRect.x + windowRect.width + 4) + 4)), 24
    };
    Rectangle singleOptionRect = optionsRect;
    singleOptionRect.width /= 2;
    //singleOptionRect.width -= 4;
    GuiToggleGroup(singleOptionRect, "Place;Erase", &actionModeToggle);

    Rectangle layoutDropdown = optionsRect;
    layoutDropdown.y += layoutDropdown.height + 4;
    if (GuiDropdownBox(layoutDropdown, "Tiles;Collisions;Interactables", &layoutModeToggle, layoutDropdownEditMode)) {
        layoutDropdownEditMode = !layoutDropdownEditMode;
    }
}
