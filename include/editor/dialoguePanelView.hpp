#ifndef _RPGPP_EDITOR_DIALOGUEPANELVIEW_H
#define _RPGPP_EDITOR_DIALOGUEPANELVIEW_H

#include "dialogueViewer.hpp"
#include <raylib.h>

class DialoguePanelView {
private:
    Rectangle rect;
    DialogueViewer view;
public:
    DialoguePanelView();
    DialoguePanelView(Rectangle rect);
    void setInitial();
    void setRect(Rectangle rect);
    void update();
    void draw();
};

#endif
