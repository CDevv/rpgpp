#include "panelView.hpp"
#include "dialoguePanelView.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "mapPanelView.hpp"
#include "projectFile.hpp"
#include "propPanelView.hpp"
#include "tileSetPanelView.hpp"

PanelView::PanelView() {}

PanelView::PanelView(Rectangle rect)
{
    this->rect = rect;
    this->pos = Vector2 { rect.x, rect.y };

    this->tileSetPanel = TileSetPanelView(rect);
    this->mapPanel = MapPanelView(rect);
    this->actorPanel = ActorPanelView(rect);
    this->dialoguePanel = DialoguePanelView(rect);
    this->propPanel = PropPanelView(rect);
}

void PanelView::setRect(Rectangle rect)
{
    this->rect = rect;
    tileSetPanel.setRect(rect);
    mapPanel.setRect(rect);
    actorPanel.setRect(rect);
    dialoguePanel.setRect(rect);
    propPanel.setRect(rect);
}

void PanelView::setInitial()
{
    tileSetPanel.setInitial();
    mapPanel.setInitial();
    actorPanel.setInitial();
    dialoguePanel.setInitial();
    propPanel.setInitial();
}

void PanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();
    tileSetPanel.update();
    mapPanel.update();
    actorPanel.update();
    dialoguePanel.update();
    propPanel.update();
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
        case FILE_DIALOGUE:
            dialoguePanel.draw();
            break;
        case FILE_PROP:
            propPanel.draw();
            break;
        }
    }
}
