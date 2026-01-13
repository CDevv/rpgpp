#ifndef _RPGPP_ABOUTWINDOW_H
#define _RPGPP_ABOUTWINDOW_H

#include <raylib.h>
#include "windowPopup.hpp"

class AboutWindow : public WindowPopup {
private:
    Rectangle rect{};
public:
    AboutWindow();
    AboutWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
