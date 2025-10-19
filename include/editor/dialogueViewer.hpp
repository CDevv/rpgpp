#ifndef _RPGPP_EDITOR_DIALOGUEVIEWER_H
#define _RPGPP_EDITOR_DIALOGUEVIEWER_H

#include <raylib.h>
#include "dialogueBalloon.hpp"
#include <Str.h>

class DialogueViewer {
private:
    Rectangle rect;
    Dialogue* diag;
    //Str256 str;
public:
    DialogueViewer();
    DialogueViewer(Rectangle rect);
    void setRect(Rectangle rect);
    void setDialogue(Dialogue* diag);
    void update();
    void draw();
};

#endif
