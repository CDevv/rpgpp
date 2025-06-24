#include "tileSetPanelView.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "propertiesBox.hpp"
#include <raylib.h>

TileSetPanelView::TileSetPanelView()
{
}

TileSetPanelView::TileSetPanelView(Rectangle rect)
{
    Rectangle windowRect = Rectangle
    {
        176, 48,
        static_cast<float>(GetScreenWidth() - 386), static_cast<float>(GetScreenHeight() - 56)
    };
    Rectangle renderRect = Rectangle {
        (windowRect.x + 2), (windowRect.y + 24),
        (windowRect.width - 4), (windowRect.height - 30)
    };
    worldView = std::make_unique<WorldViewBox>(windowRect, renderRect, FILE_TILESET);

    Rectangle propRect = Rectangle
    {
        (windowRect.x + windowRect.width + 8), (windowRect.y)
    };
    propRect.width = GetScreenWidth() - propRect.x - 8;
    propRect.height = windowRect.height;
    propBox = PropertiesBox(propRect);
}

void TileSetPanelView::setInitial()
{
    propBox.setDefaults();
}

void TileSetPanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();
    worldView->setTileSet(fs.getTileSet());
    worldView->update();
    propBox.update();
}

void TileSetPanelView::draw()
{
    worldView->draw();
    propBox.draw();
}
