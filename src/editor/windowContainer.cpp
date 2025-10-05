#include "windowContainer.hpp"
#include "mapInitWindow.hpp"
#include "tileSetInitWindow.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "worldViewBox.hpp"
#include <raylib.h>

WindowContainer::WindowContainer()
{
    Rectangle baseTileSetSize = Rectangle { 0, 0, 312, 184 };
    Rectangle tileSetWindowSize = Rectangle {
        (GetScreenWidth() - baseTileSetSize.width) / 2,
        (GetScreenHeight() - baseTileSetSize.height) / 2,
        baseTileSetSize.width, baseTileSetSize.height
    };
    tileSetInit = TileSetInitWindow(tileSetWindowSize);

    Rectangle mapWindowSize = Rectangle(tileSetWindowSize);
    mapInit = MapInitWindow(mapWindowSize);

    actorInit = ActorInitWindow(tileSetWindowSize);

    projectInit = ProjectInitWindow(tileSetWindowSize);

    Rectangle baseProjectViewWindowSize = Rectangle
    {
        0, 0, static_cast<float>(GetScreenWidth() - 64), static_cast<float>(GetScreenHeight() - 64)
    };
    Rectangle projectViewWindowRect = Rectangle
    {
        (GetScreenWidth() - baseProjectViewWindowSize.width) / 2,
        (GetScreenHeight() - baseProjectViewWindowSize.height) / 2,
        baseProjectViewWindowSize.width, baseProjectViewWindowSize.height
    };
    projectBinaryView = ProjectBinaryViewWindow(projectViewWindowRect);

    Rectangle baseTileSetDialogSize = Rectangle {
        0, 0, 434, 404
    };
    Rectangle tileSetDialogSize = Rectangle {
        (GetScreenWidth() - baseTileSetDialogSize.width) / 2,
        (GetScreenHeight() - baseTileSetDialogSize.height) / 2,
        baseTileSetDialogSize.width, baseTileSetDialogSize.height
    };
    tileSetDialog = TileSetDialogWindow(tileSetDialogSize);
}

void WindowContainer::update()
{
    tileSetDialog.update();
}

bool WindowContainer::isWindowOpen()
{
    return this->windowOpen;
}

void WindowContainer::setWindowOpen(bool value)
{
    this->windowOpen = value;
}

void WindowContainer::openProjectInit()
{
    windowOpen = true;
    projectInit.setActive();
}

void WindowContainer::openTileSetInit()
{
    windowOpen = true;
    tileSetInit.setActive();
}

void WindowContainer::openMapInit()
{
    windowOpen = true;
    mapInit.setActive();
}

void WindowContainer::openActorInit()
{
    windowOpen = true;
    actorInit.setActive();
}

ProjectBinaryViewWindow& WindowContainer::openProjectBinaryView()
{
    windowOpen = true;
    projectBinaryView.setActive();
    return projectBinaryView;
}

TileSetDialogWindow& WindowContainer::openTileSetDialog()
{
    EditorInterfaceService& ui = Editor::getUi();
    ui.setMouseBoxLayer(VIEWBOX_LAYER_DIALOG);
    windowOpen = true;
    tileSetDialog.setActive();
    return tileSetDialog;
}

void WindowContainer::draw()
{
    //projectInit.draw();
    tileSetInit.draw();
    mapInit.draw();
    actorInit.draw();
    //projectBinaryView.draw();
    //tileSetDialog.draw();

    if (!windowOpen) {
        EditorInterfaceService& ui = Editor::getUi();
        ui.setMouseLock(false);
    }
}

void WindowContainer::drawTileSetDialog()
{
    tileSetDialog.draw();
}

void WindowContainer::drawProjectInit()
{
    projectInit.draw();
}

void WindowContainer::drawProjectBinaryView()
{
    projectBinaryView.draw();
}
