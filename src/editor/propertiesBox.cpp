#include "propertiesBox.hpp"
#include <raygui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "mapPropertiesBox.hpp"
#include "tileSetPropertiesBox.hpp"

PropertiesBox::PropertiesBox() {}

PropertiesBox::PropertiesBox(Rectangle rect)
{
    this->pos = Vector2 { rect.x, rect.y };
    this->rect = rect;

    tileSetProps = TileSetPropertiesBox(rect);
    mapProps = MapPropertiesBox(rect);
}

void PropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {
        switch (fs.getType()) {
        default:
            break;
        case FILE_TILESET:
            tileSetProps.setDefaults();
            break;
        case FILE_ROOM:
            mapProps.setDefaults();
            break;
        }
    }
}

void PropertiesBox::update()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {
        switch (fs.getType()) {
        default:
            break;
        case FILE_TILESET:
            tileSetProps.update();
            break;
        case FILE_ROOM:
            mapProps.update();
            break;
        }
    }
}

void PropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {
        switch (fs.getType()) {
        default:
            break;
        case FILE_TILESET:
            tileSetProps.draw();
            break;
        case FILE_ROOM:
            mapProps.draw();
            break;
        }
    }
}
