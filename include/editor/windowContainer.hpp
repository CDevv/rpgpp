#ifndef _RPGPP_EDITOR_WINDOWCONTAINER_H
#define _RPGPP_EDITOR_WINDOWCONTAINER_H

#include "tileSetInitWindow.hpp"
#include "mapInitWindow.hpp"
#include "projectBinaryViewWindow.hpp"
#include "tileSetDialogWindow.hpp"
#include "actorInitWindow.hpp"

class WindowContainer {
private:
    bool windowOpen;
    TileSetInitWindow tileSetInit;
    MapInitWindow mapInit;
    ActorInitWindow actorInit;
    ProjectBinaryViewWindow projectBinaryView;
    TileSetDialogWindow tileSetDialog;
public:
    WindowContainer();
    bool isWindowOpen();
    void setWindowOpen(bool value);
    void openTileSetInit();
    void openMapInit();
    void openActorInit();
    ProjectBinaryViewWindow& openProjectBinaryView();
    TileSetDialogWindow& openTileSetDialog();
    void draw();
};

#endif