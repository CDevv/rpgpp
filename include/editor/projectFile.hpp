#ifndef _RPGPP_EDITOR_PROJECTFILE_H
#define _RPGPP_EDITOR_PROJECTFILE_H

#include <memory>
#include <string>
#include "tileset.hpp"
#include "room.hpp"
#include "actor.hpp"

enum EngineFileType {
    FILE_TILESET,
    FILE_ROOM,
    FILE_ACTOR
};

class ProjectFile {
private:
    std::string relativePath;
    EngineFileType fileType;
    static std::array<std::string, 3> fileTypeNames;
    std::unique_ptr<TileSet> tileSet;
    std::unique_ptr<Room> room;
    std::unique_ptr<Actor> actor;
public:
    ProjectFile();
    ProjectFile(std::string relativePath, EngineFileType fileType);
    static std::array<std::string, 3> getTypeNames();
    void setFromPath(std::string relativePath);
    std::string getRelativePath();
    EngineFileType getFileType();
    void setTileSet(TileSet* tileSet);
    TileSet* getTileSet();
    void setRoom(Room* room);
    Room* getRoom();
    void setActor(Actor* actor);
    Actor* getActor();
};

#endif