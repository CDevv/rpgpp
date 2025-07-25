#include "windowContainer.hpp"
#include "mapInitWindow.hpp"
#include "tileSetInitWindow.hpp"
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
}

void WindowContainer::openTileSetInit()
{
    tileSetInit.setActive();
}

void WindowContainer::openMapInit()
{
    mapInit.setActive();
}

ProjectBinaryViewWindow& WindowContainer::openProjectBinaryView()
{
    projectBinaryView.setActive();
    return projectBinaryView;
}

void WindowContainer::draw()
{
    tileSetInit.draw();
    mapInit.draw();
    projectBinaryView.draw();
}