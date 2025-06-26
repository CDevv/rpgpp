#ifndef _RPGPP_EDITOR_WINDOWCONTAINER_H
#define _RPGPP_EDITOR_WINDOWCONTAINER_H

#include "tileSetInitWindow.hpp"

class WindowContainer {
private:
    TileSetInitWindow tileSetInit;
public:
    WindowContainer();
    void openTileSetInit();
    void draw();
};

#endif