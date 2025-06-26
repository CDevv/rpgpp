#ifndef _RPGPP_EDITOR_TILESETINITWINDOW_H
#define _RPGPP_EDITOR_TILESETINITWINDOW_H

#include <raylib.h>
#include <string>

class TileSetInitWindow {
private:
    bool active;
    Rectangle rect;
    bool titleEditMode;
    std::string titleText;
    char *title;
    bool hasSetTextureSource;
    std::string textureSource;
public:
    TileSetInitWindow();
    TileSetInitWindow(Rectangle rect);
    void setActive();
    void draw();
};

#endif