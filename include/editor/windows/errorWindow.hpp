#ifndef _RPGPP_EDITOR_ERRORWINDOW_H
#define _RPGPP_EDITOR_ERRORWINDOW_H

#include <raylib.h>
#include "windowPopup.hpp"

class ErrorWindow : public WindowPopup {
private:
    Rectangle rect;
public:
    ErrorWindow();
    ErrorWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
