#ifndef _RPGPP_EDITOR_DIALOGUELINEWINDOW_H
#define _RPGPP_EDITOR_DIALOGUELINEWINDOW_H

#include "windowPopup.hpp"
#include <raylib.h>

class DialogueLineWindow : public WindowPopup {
private:
    Rectangle rect;
    char lineTitle[256];
public:
    DialogueLineWindow();
    DialogueLineWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
