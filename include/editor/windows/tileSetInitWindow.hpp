#ifndef _RPGPP_EDITOR_TILESETINITWINDOW_H
#define _RPGPP_EDITOR_TILESETINITWINDOW_H

#include <raylib.h>
#include <string>
#include "windowPopup.hpp"

class TileSetInitWindow : public WindowPopup {
private:
    Rectangle rect;
    bool titleEditMode;
    std::string titleText;
    char title[256];
    bool hasSetTextureSource;
    std::string textureSource;
public:
    TileSetInitWindow();
    TileSetInitWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
