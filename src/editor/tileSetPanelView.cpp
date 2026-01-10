#include "tileSetPanelView.hpp"

#include <raylib.h>

#include "editor.hpp"
#include "fileSystemService.hpp"
#include "projectFile.hpp"
#include "propertiesBox.hpp"
#include "tileset.hpp"

TileSetPanelView::TileSetPanelView() : rect()
{
}

TileSetPanelView::TileSetPanelView(Rectangle rect)
{
    this->rect = Rectangle {};
    Rectangle windowRect = Rectangle
    {
        rect.x, rect.y,
        static_cast<float>(rect.width - 190), rect.height
    };
    worldView = std::make_unique<WorldViewBox>(windowRect, FILE_TILESET, VIEWBOX_LAYER_BASE);

    Rectangle propRect = Rectangle
    {
        (windowRect.x + windowRect.width + 4), (windowRect.y)
    };
    propRect.width = rect.width - windowRect.width - 4;
    propRect.height = windowRect.height + 2;
    propBox = PropertiesBox(propRect);
}

void TileSetPanelView::setRect(Rectangle rect)
{
    this->rect = rect;
    Rectangle windowRect = Rectangle
    {
        rect.x, rect.y,
        static_cast<float>(rect.width * 0.7), rect.height
    };
    worldView->setRect(windowRect);

    Rectangle propRect = Rectangle
    {
        (windowRect.x + windowRect.width + 4), (windowRect.y)
    };
    propRect.width = rect.width - windowRect.width - 4;
    propRect.height = windowRect.height;
    propBox.setRect(propRect);
}

void TileSetPanelView::setInitial()
{
    FileSystemService& fs = Editor::getFileSystem();
    if (fs.fileIsOpen() && fs.getType() == FILE_TILESET) {
        worldView->setTileSet(fs.getCurrentFile()->getData<TileSet>());
    }
    propBox.setDefaults();
}

void TileSetPanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

    worldView->setMouseLock(ui.getMouseLock());

    worldView->update();
    if (fs.fileIsOpen() && fs.getType() == FILE_TILESET) {
        propBox.update();
    }
}

void TileSetPanelView::draw()
{
    worldView->draw();
    propBox.draw();
}
