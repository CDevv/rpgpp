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
}

void PanelView::setInitial()
{
    tileSetPanel.setInitial();
    mapPanel.setInitial();
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
        if (fs.fileIsOpen()) {
            if (fs.getType() == FILE_TILESET) {
            tileSetPanel.draw();
        } else {
            mapPanel.draw();
        }
    }
}

