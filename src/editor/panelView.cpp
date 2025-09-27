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
    this->actorPanel = ActorPanelView(rect);
}

void PanelView::setRect(Rectangle rect)
{
    this->rect = rect;
    tileSetPanel.setRect(rect);
    mapPanel.setRect(rect);
}

void PanelView::setInitial()
{
    tileSetPanel.setInitial();
    mapPanel.setInitial();
    actorPanel.setInitial();
}

void PanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();
    tileSetPanel.update();
    mapPanel.update();
    actorPanel.update();
}

void PanelView::draw()
{
    this->setRect(rect);

    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {

        switch (fs.getType()) {
        default:
            break;
        case FILE_TILESET:
            tileSetPanel.draw();
            break;
        case FILE_ROOM:
            mapPanel.draw();
            break;
        case FILE_ACTOR:
            actorPanel.draw();
            break;
        }
    }
}

