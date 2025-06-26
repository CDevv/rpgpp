#include "windowContainer.hpp"
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
}

void WindowContainer::openTileSetInit()
{
    tileSetInit.setActive();
}

void WindowContainer::draw()
{
    tileSetInit.draw();
}