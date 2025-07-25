#ifndef _RPGPP_EDITOR_WINDOWCONTAINER_H
#define _RPGPP_EDITOR_WINDOWCONTAINER_H

#include "tileSetInitWindow.hpp"
#include "mapInitWindow.hpp"
#include "projectBinaryViewWindow.hpp"

class WindowContainer {
private:
    TileSetInitWindow tileSetInit;
    MapInitWindow mapInit;
    ProjectBinaryViewWindow projectBinaryView;
public:
    WindowContainer();
    void openTileSetInit();
    void openMapInit();
    ProjectBinaryViewWindow& openProjectBinaryView();
    void draw();
};

#endif