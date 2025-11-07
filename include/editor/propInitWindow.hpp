#ifndef _RPGPP_EDITOR_PROPINITWINDOW_H
#define _RPGPP_EDITOR_PROPINITWINDOW_H

#include <raylib.h>
#include "windowPopup.hpp"

class PropInitWindow : public WindowPopup {
private:
    Rectangle rect;
    char title[256];
    std::string imageSourcePath;
    bool hasSetImageSource;
public:
    PropInitWindow();
    PropInitWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
