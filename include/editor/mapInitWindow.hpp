#ifndef _RPGPP_EDITOR_MAPINITWINDOW_H
#define _RPGPP_EDITOR_MAPINITWINDOW_H

#include <raylib.h>
#include <string>
#include <memory>

class MapInitWindow {
private:
    bool active;
    Rectangle rect;
    std::string titleText;
    std::unique_ptr<char> title;
    bool titleEditMode;
    std::string tileSetSource;
    bool hasSetTileSet;
public:
    MapInitWindow();
    MapInitWindow(Rectangle rect);
    void setActive();
    void closeWindow();
    void draw();
};

#endif