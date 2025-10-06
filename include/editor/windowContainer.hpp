#ifndef _RPGPP_EDITOR_WINDOWCONTAINER_H
#define _RPGPP_EDITOR_WINDOWCONTAINER_H

#include "projectInitWindow.hpp"
#include "tileSetInitWindow.hpp"
#include "mapInitWindow.hpp"
#include "projectBinaryViewWindow.hpp"
#include "tileSetDialogWindow.hpp"
#include "actorInitWindow.hpp"
#include "deleteConfirmWindow.hpp"

class WindowContainer {
private:
    bool windowOpen;
    ProjectInitWindow projectInit;
    TileSetInitWindow tileSetInit;
    MapInitWindow mapInit;
    ActorInitWindow actorInit;
    ProjectBinaryViewWindow projectBinaryView;
    TileSetDialogWindow tileSetDialog;
    DeleteConfirmWindow deleteConfirm;
public:
    WindowContainer();
    bool isWindowOpen();
    void setWindowOpen(bool value);
    void openProjectInit();
    void openTileSetInit();
    void openMapInit();
    void openActorInit();
    ProjectBinaryViewWindow& openProjectBinaryView();
    TileSetDialogWindow& openTileSetDialog();
    DeleteConfirmWindow& openDeleteConfirm();
    void update();
    void draw();
    void drawTileSetDialog();
    void drawProjectInit();
    void drawProjectBinaryView();
    void drawDeleteConfirm();
};

#endif
