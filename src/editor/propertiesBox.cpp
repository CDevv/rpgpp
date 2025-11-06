#include "propertiesBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "mapPropertiesBox.hpp"
#include "projectFile.hpp"
#include "propPropertiesBox.hpp"
#include "tileSetPropertiesBox.hpp"

PropertiesBox::PropertiesBox() {}

PropertiesBox::PropertiesBox(Rectangle rect)
{
    this->pos = Vector2 { rect.x, rect.y };
    this->rect = rect;

    tileSetProps = TileSetPropertiesBox(rect);
    mapProps = MapPropertiesBox(rect);
    actorProps = ActorPropertiesBox(rect);
    propProps = PropPropertiesBox();
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
        case FILE_PROP:
            propProps.setDefaults();
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
        case FILE_PROP:
            propProps.update();
            break;
        }
    }
}

void PropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("Props", NULL,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Tooltip)) {

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
            case FILE_PROP:
                propProps.draw();
                break;
            }
        }

        ImGui::End();
    }
}

ActorPropertiesBox& PropertiesBox::getActorProps()
{
    return actorProps;
}

PropPropertiesBox& PropertiesBox::getPropProps()
{
    return propProps;
}
