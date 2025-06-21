#include "tileSetPanelView.hpp"
#include "fileSystemService.hpp"

TileSetPanelView::TileSetPanelView()
{
}

TileSetPanelView::TileSetPanelView(Rectangle rect)
{
    Rectangle windowRect = Rectangle
    {
        176, 48,
        static_cast<float>(GetScreenWidth() - 386), static_cast<float>(GetScreenHeight() - 56)
    };
    Rectangle renderRect = Rectangle {
        (windowRect.x + 2), (windowRect.y + 24),
        (windowRect.width - 4), (windowRect.height - 30)
    };
    worldView = std::make_unique<WorldViewBox>(windowRect, renderRect, FILE_TILESET);
}

void TileSetPanelView::update()
{
    worldView->update();
}

void TileSetPanelView::draw()
{
    worldView->draw();
}
