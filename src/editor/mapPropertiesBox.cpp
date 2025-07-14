#include "mapPropertiesBox.hpp"
#include <raygui.h>
#include <raylib.h>
#include "editor.hpp"
#include "fileSystemService.hpp"

MapPropertiesBox::MapPropertiesBox() {}

MapPropertiesBox::MapPropertiesBox(Rectangle rect)
{
    this->rect = rect;

    mapWidth = 0;
    widthEdit = false;

    mapHeight = 0;
    heightEdit = false;

    scrollVec = Vector2 { 0 ,0 };
    viewRec = Rectangle { 0 };
}

void MapPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();
    Room *room = fs.getRoom();
    this->room = room;
    TileMap *map = room->getTileMap();
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
    TileMap *map = fs.getRoom()->getTileMap();
    this->map = map;

    Rectangle contentRec = rect;
    contentRec.height += 100;
    contentRec.width -= 16;
    GuiScrollPanel(rect, "Map Props", contentRec, &scrollVec, &viewRec);

    BeginScissorMode(viewRec.x, viewRec.y, viewRec.width, viewRec.height);

    // map size
    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 8, (viewRec.width - 16), 24 }, "Width");
    if (GuiValueBox(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 32, (viewRec.width - 16), 24 }, NULL, &mapWidth, 1, 100, widthEdit)) {
        widthEdit = !widthEdit;
    }

    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 56, (viewRec.width - 16), 24 }, "Height");
    if (GuiValueBox(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 80, (viewRec.width - 16), 24 }, NULL, &mapHeight, 1, 100, heightEdit)) {
        heightEdit = !heightEdit;
    }

    if (GuiButton(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 112, (viewRec.width - 16), 24 }, "Resize")) {
        if (mapHeight >= 1 && mapWidth >= 1) {
            if (mapHeight <= 75 && mapWidth <= 75) {
                map->setWorldSize(Vector2 { static_cast<float>(mapWidth), static_cast<float>(mapHeight) });
            }
        }
    }

    GuiLine(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 136, (viewRec.width - 16), 16 }, NULL);
    // tileset source
    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 152, (viewRec.width - 16), 24 }, "TileSet");
    std::string sourceFileName = GetFileName(map->getTileSetSource().c_str());
    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 176, (viewRec.width - (16 + 24)), 24 }, sourceFileName.c_str());
    if (GuiButton(Rectangle { viewRec.x + 8 + (viewRec.width - (16 + 24)), viewRec.y + scrollVec.y + 176, 24, 24 }, GuiIconText(ICON_FILE_OPEN, NULL))) {
        FS_Result fsResult = fs.openTileSetResource();
        map->setTileSet(fsResult.path);
    }

    EndScissorMode();

    //draw tooltips AFTER scrissor mode
    EditorInterfaceService& ui = Editor::getUi();
    ui.drawTooltip(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 176, (viewRec.width - (16 + 24)), 24 }, map->getTileSetSource());
}

