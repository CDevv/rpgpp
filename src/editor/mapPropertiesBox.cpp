#include "mapPropertiesBox.hpp"
#include <raygui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"

MapPropertiesBox::MapPropertiesBox()
{
    mapWidth = 0;
    widthEdit = false;

    mapHeight = 0;
    heightEdit = false;
}

void MapPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();
    TileMap *map = fs.getTileMap();
    this->map = map;

    Vector2 mapSize = map->getMaxWorldSize();
    this->mapWidth = mapSize.x;
    this->mapHeight = mapSize.y;
}

void MapPropertiesBox::update()
{
}

void MapPropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    TileMap *map = fs.getTileMap();
    this->map = map;

    GuiPanel(Rectangle { 8, 48, 160, static_cast<float>(GetScreenHeight() - 56) }, "TileMap Props");

    // map size
    GuiLabel(Rectangle { 16, 80, 144, 24 }, "Width");
    if (GuiValueBox(Rectangle { 16, 104, 144, 24 }, NULL, &mapWidth, 1, 100, widthEdit)) {
        widthEdit = !widthEdit;
    }

    GuiLabel(Rectangle { 16, 136, 144, 24 }, "Height");
    if (GuiValueBox(Rectangle { 16, 160, 144, 24 }, NULL, &mapHeight, 1, 100, heightEdit)) {
        heightEdit = !heightEdit;
    }

    if (GuiButton(Rectangle { 16, 192, 144, 24 }, "Resize")) {
        if (mapHeight >= 1 && mapWidth >= 1) {
            if (mapHeight <= 75 && mapWidth <= 75) {
                map->setWorldSize(Vector2 { static_cast<float>(mapWidth), static_cast<float>(mapHeight) });
            }
        }
    }

    GuiLine(Rectangle { 16, 216, 144, 16 }, NULL);
    // tileset source
    GuiLabel(Rectangle { 16, 236, 144, 24 }, "TileSet");
    std::string sourceFileName = GetFileName(map->getTileSetSource().c_str());
    GuiLabel(Rectangle { 16, 256, 144, 24 }, sourceFileName.c_str());
    if (GuiButton(Rectangle { 16, 280, 144, 24 }, "Change TileSet")) {
        FS_Result fsResult = fs.openTileSetResource();
        map->setTileSet(fsResult.path);
    }

    EditorInterfaceService& ui = Editor::getUi();
    ui.drawTooltip(Rectangle { 16, 256, 144, 24 }, map->getTileSetSource());
}

