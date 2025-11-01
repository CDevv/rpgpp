#include "mapPropertiesBox.hpp"
#include <raylib.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "projectFile.hpp"
#include "room.hpp"

MapPropertiesBox::MapPropertiesBox() {}

MapPropertiesBox::MapPropertiesBox(Rectangle rect)
{
    this->room = nullptr;
    this->map = nullptr;

    this->rect = rect;

    mapWidth = 0;
    widthEdit = false;

    mapHeight = 0;
    heightEdit = false;

    musicOn = false;

    scrollVec = Vector2 { 0 ,0 };
    viewRec = Rectangle { 0 };
}

void MapPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.isAvailable(FILE_ROOM)) {
        Room *room = fs.getCurrentFile()->getData<Room>();
        this->room = room;
        TileMap *map = room->getTileMap();
        this->map = map;

        Vector2 mapSize = map->getMaxWorldSize();
        this->mapWidth = mapSize.x;
        this->mapHeight = mapSize.y;

        if (!room->getMusicSource().empty()) {
            this->musicOn = true;
        }
    }
}

void MapPropertiesBox::update()
{
}

void MapPropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    TileMap *map = fs.getCurrentFile()->getData<Room>()->getTileMap();
    this->map = map;

    ImGui::SeparatorText("Map Size");
    ImGui::InputInt("Width", &mapWidth);
    ImGui::InputInt("Height", &mapHeight);
    if (ImGui::Button("Resize", ImVec2(-1, 0))) {
        if (mapHeight >= 1 && mapWidth >= 1) {
            if (mapHeight <= 75 && mapWidth <= 75) {
                map->setWorldSize(Vector2 { static_cast<float>(mapWidth), static_cast<float>(mapHeight) });
            }
        }
    }

    std::string sourceFileName = GetFileName(map->getTileSetSource().c_str());
    sourceFileName.push_back('\0');

    ImGui::SeparatorText("TileSet");
    ImGui::InputText("TileSet", const_cast<char*>(sourceFileName.data()), sourceFileName.size(), ImGuiInputTextFlags_ReadOnly);
    if (ImGui::Button("Change TileSet..", ImVec2(-1, 0))) {
        FS_Result fsResult = fs.openTileSetResource();
        if (fsResult.result == NFD_OKAY) {
            map->setTileSet(fsResult.path);
        }
    }

    ImGui::SeparatorText("Music");
    ImGui::Checkbox("Has BG Music", &musicOn);

    if (musicOn) {
        std::string musicSource = room->getMusicSource();
        musicSource.push_back('\0');

        ImGui::InputText("Music Source", const_cast<char*>(musicSource.data()), musicSource.size(),
            ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("Change Music..", ImVec2(-1, 0))) {
            FS_Result fsResult = fs.openMusic();
            if (fsResult.result == NFD_OKAY) {
                room->setMusicSource(fsResult.fileName);
            }
        }
    } else {
        room->setMusicSource("");
    }
}
