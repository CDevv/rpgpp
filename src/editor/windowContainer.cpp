#include "windowContainer.hpp"
#include <memory>
#include <raylib.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "windowPopup.hpp"
#include "worldViewBox.hpp"

#include "windows/projectInitWindow.hpp"
#include "windows/tileSetInitWindow.hpp"
#include "windows/mapInitWindow.hpp"
#include "windows/actorInitWindow.hpp"
#include "windows/projectBinaryViewWindow.hpp"
#include "windows/tileSetDialogWindow.hpp"
#include "windows/deleteConfirmWindow.hpp"
#include "windows/aboutWindow.hpp"
#include "windows/errorWindow.hpp"

WindowContainer::WindowContainer()
{
    Rectangle baseTileSetSize = Rectangle { 0, 0, 312, 184 };
    Rectangle tileSetWindowSize = Rectangle {
        (GetScreenWidth() - baseTileSetSize.width) / 2,
        (GetScreenHeight() - baseTileSetSize.height) / 2,
        baseTileSetSize.width, baseTileSetSize.height
    };

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

    mapW = std::map<std::string, std::unique_ptr<WindowPopup>>{};

    mapW.emplace("ProjectInit", std::unique_ptr<WindowPopup>(new ProjectInitWindow(tileSetWindowSize)));
    mapW.emplace("TileSetInit", std::unique_ptr<WindowPopup>(new TileSetInitWindow(tileSetWindowSize)));
    mapW.emplace("MapInit", std::unique_ptr<WindowPopup>(new MapInitWindow(tileSetWindowSize)));
    mapW.emplace("ActorInit", std::unique_ptr<WindowPopup>(new ActorInitWindow(tileSetWindowSize)));
    mapW.emplace("About",  std::unique_ptr<WindowPopup>(new AboutWindow(tileSetWindowSize)));
    mapW.emplace("Error",  std::unique_ptr<WindowPopup>(new ErrorWindow(Rectangle { 20, 20, 240, 30 })));
    mapW.emplace("DeleteConfirm",  std::unique_ptr<WindowPopup>(new DeleteConfirmWindow(tileSetWindowSize)));
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
    drawWindow("TileSetInit");
    drawWindow("MapInit");
    drawWindow("ActorInit");

    if (!windowOpen) {
        EditorInterfaceService& ui = Editor::getUi();
        ui.setMouseLock(false);
    }
}

void WindowContainer::drawTileSetDialog()
{
    tileSetDialog.draw();
}

void WindowContainer::drawProjectBinaryView()
{
    projectBinaryView.draw();
}

WindowPopup& WindowContainer::open(std::string id)
{
    mapW[id]->openWindow();
    return *mapW[id].get();
}

void WindowContainer::drawWindow(std::string id)
{
    mapW[id]->draw();
}
