#ifndef _RPGPP_EDITOR_DIALOGUEVIEWER_H
#define _RPGPP_EDITOR_DIALOGUEVIEWER_H

#include <map>
#include <raylib.h>
#include "dialogueBalloon.hpp"

class DialogueViewer {
private:
    Rectangle rect;
    Dialogue* diag;
    char newLineName[256];
    std::map<std::string, Texture2D> images;
    bool drawDialogueLine(int idx, std::vector<DialogueLine>::iterator it);
public:
    DialogueViewer();
    DialogueViewer(Rectangle rect);
    void setRect(Rectangle rect);
    void setDialogue(Dialogue* diag);
    void update();
    void draw();
};

#endif
