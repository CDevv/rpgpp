#ifndef _RPGPP_ABOUTWINDOW_H
#define _RPGPP_ABOUTWINDOW_H

#include <raylib.h>

class AboutWindow {
private:
    bool active;
    Rectangle rect;
public:
    AboutWindow();
    AboutWindow(Rectangle rect);
    void setActive();
    void closeWindow();
    void draw();
};

#endif
