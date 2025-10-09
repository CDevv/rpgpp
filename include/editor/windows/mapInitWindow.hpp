#ifndef _RPGPP_EDITOR_MAPINITWINDOW_H
#define _RPGPP_EDITOR_MAPINITWINDOW_H

#include <raylib.h>
#include <string>
#include "windowPopup.hpp"

class MapInitWindow : public WindowPopup {
private:
    Rectangle rect;
    std::string titleText;
    char title[256];
    bool titleEditMode;
    std::string tileSetSource;
    bool hasSetTileSet;
public:
    MapInitWindow();
    MapInitWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
