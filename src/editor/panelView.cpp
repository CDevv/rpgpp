#include "panelView.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "mapPanelView.hpp"
#include "tileSetPanelView.hpp"

PanelView::PanelView() {}

PanelView::PanelView(Rectangle rect)
{
    this->rect = rect;
    this->pos = Vector2 { rect.x, rect.y };

    this->tileSetPanel = TileSetPanelView(rect);
    this->mapPanel = MapPanelView(rect);

    Rectangle windowRect = Rectangle
    {
        176, 48,
        static_cast<float>(GetScreenWidth() - 386), static_cast<float>(GetScreenHeight() - 56)
    };
    Rectangle renderRect = Rectangle {
        (windowRect.x + 2), (windowRect.y + 24),
        (windowRect.width - 4), (windowRect.height - 30)
    };
}

void PanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();
    tileSetPanel.update();
    mapPanel.update();
}

void PanelView::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    if (fs.getType() == FILE_TILESET) {
        tileSetPanel.draw();
    } else {
        mapPanel.draw();
    }
}

