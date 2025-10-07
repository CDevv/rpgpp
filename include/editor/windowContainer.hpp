#ifndef _RPGPP_EDITOR_WINDOWCONTAINER_H
#define _RPGPP_EDITOR_WINDOWCONTAINER_H

#include "windows/projectInitWindow.hpp"
#include "windows/tileSetInitWindow.hpp"
#include "windows/mapInitWindow.hpp"
#include "windows/projectBinaryViewWindow.hpp"
#include "windows/tileSetDialogWindow.hpp"
#include "windows/actorInitWindow.hpp"
#include "windows/deleteConfirmWindow.hpp"
#include "windows/aboutWindow.hpp"

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
    AboutWindow about;
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
    void openAbout();
    void update();
    void draw();
    void drawTileSetDialog();
    void drawProjectInit();
    void drawProjectBinaryView();
    void drawDeleteConfirm();
    void drawAbout();
};

#endif
