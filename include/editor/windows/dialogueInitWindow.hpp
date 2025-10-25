#ifndef _RPGPP_EDITOR_DIALOGUEINITWINDOW_H
#define _RPGPP_EDITOR_DIALOGUEINITWINDOW_H

#include <raylib.h>
#include "windowPopup.hpp"

class DialogueInitWindow : public WindowPopup {
private:
    Rectangle rect;
    char title[256];
    std::string titleText;
public:
    DialogueInitWindow();
    DialogueInitWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
