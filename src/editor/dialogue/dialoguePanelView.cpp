#include "dialoguePanelView.hpp"

#include "dialogueBalloon.hpp"
#include "dialogueViewer.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "projectFile.hpp"

DialoguePanelView::DialoguePanelView() : rect()
{
}

DialoguePanelView::DialoguePanelView(Rectangle rect)
{
    this->rect = rect;
    view = DialogueViewer(rect);
}

void DialoguePanelView::setInitial()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.isAvailable(FILE_DIALOGUE)) {
        view.setDialogue(fs.getCurrentFile()->getData<Dialogue>());
    }
}

void DialoguePanelView::setRect(Rectangle rect)
{
    this->rect = rect;
    view.setRect(rect);
}

void DialoguePanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.isAvailable(FILE_DIALOGUE)) {
        view.update();
    }
}

void DialoguePanelView::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.isAvailable(FILE_DIALOGUE)) {
        view.draw();
    }
}
