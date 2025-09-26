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
    actorProps = ActorPropertiesBox(rect);
}

void PropertiesBox::setRect(Rectangle rect)
{
    this->rect = rect;
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
        case FILE_ACTOR:
            actorProps.setDefaults();
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
        case FILE_ACTOR:
            actorProps.update();
            break;
        }
    }
}

void PropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("Props", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Tooltip)) {
        
        if (fs.getType() == FILE_TILESET) {
            tileSetProps.draw();
        }

        ImGui::End();
    }

    /*
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
        case FILE_ACTOR:
            actorProps.draw();
            break;
        }
    }
    */
}

ActorPropertiesBox& PropertiesBox::getActorProps()
{
    return actorProps;
}
