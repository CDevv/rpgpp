#ifndef _RPGPP_EDITOR_TILESETINITWINDOW_H
#define _RPGPP_EDITOR_TILESETINITWINDOW_H

#include <raylib.h>
#include <string>
#include <memory>

class TileSetInitWindow {
private:
    bool active;
    Rectangle rect;
    bool titleEditMode;
    std::string titleText;
    std::unique_ptr<char> title;
    bool hasSetTextureSource;
    std::string textureSource;
public:
    TileSetInitWindow();
    TileSetInitWindow(Rectangle rect);
    void setActive();
    void closeWindow();
    void draw();
};

#endif