#include "mapPanelView.hpp"
#include <memory>
#include <raylib.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "propertiesBox.hpp"
#include "worldViewBox.hpp"

MapPanelView::MapPanelView()
{
}

MapPanelView::MapPanelView(Rectangle rect)
{
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
        (windowRect.x + windowRect.width + 4), (windowRect.y),
        (GetScreenWidth() - ((windowRect.x + windowRect.width + 4) + 4)), (8)
    };
    tileSetWindowRect.height = tileSetWindowRect.width;
    Rectangle tileSetRenderRect = Rectangle
    {
        (tileSetWindowRect.x + 2), (tileSetWindowRect.y + 24),
        (tileSetWindowRect.width - 4), (tileSetWindowRect.height - 30)
    };
    tileSetView = std::make_unique<WorldViewBox>(tileSetWindowRect, tileSetRenderRect, FILE_TILESET);

    tileSetView->enableTileSelection();
    worldView->enableTilePlacement();

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
}

void MapPanelView::draw()
{
    worldView->draw();
    tileSetView->draw();
    propBox.draw();
}
