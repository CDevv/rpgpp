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
        if (fs.getType() == FILE_TILESET) {
            tileSetProps.setDefaults();
        } else {
            mapProps.setDefaults();
        }
    }
}

void PropertiesBox::update()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {
        if (fs.getType() == FILE_TILESET) {
            tileSetProps.update();
        } else {
            mapProps.update();
        }
    }
}

void PropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.fileIsOpen()) {
        if (fs.getType() == FILE_TILESET) {
            tileSetProps.draw();
        } else {
            mapProps.draw();
        }
    }
}
