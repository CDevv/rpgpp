#ifndef _RPGPP_EDITOR_WINDOWCONTAINER_H
#define _RPGPP_EDITOR_WINDOWCONTAINER_H

#include "tileSetInitWindow.hpp"
#include "mapInitWindow.hpp"

class WindowContainer {
private:
    TileSetInitWindow tileSetInit;
    MapInitWindow mapInit;
public:
    WindowContainer();
    void openTileSetInit();
    void openMapInit();
    void draw();
};

#endif