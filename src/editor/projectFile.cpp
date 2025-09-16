#include "projectFile.hpp"

ProjectFile::ProjectFile() {}

ProjectFile::ProjectFile(std::string relativePath, EngineFileType fileType)
{
    this->relativePath = relativePath;
    this->fileType = fileType;
    this->tileSet = std::unique_ptr<TileSet>{};
    this->room = std::unique_ptr<Room>{};
    this->actor = std::unique_ptr<Actor>{};
}

void ProjectFile::setFromPath(std::string relativePath)
{
    std::string fileExtension = GetFileExtension(relativePath.c_str());
    if (TextIsEqual(fileExtension.c_str(), ".rtiles")) {
        tileSet.reset(new TileSet(relativePath));
        fileType = FILE_TILESET;
    } else if (TextIsEqual(fileExtension.c_str(), ".rmap")) {
        room.reset(new Room(relativePath));
        fileType = FILE_ROOM;
    } else if (TextIsEqual(fileExtension.c_str(), ".ractor")) {
        actor.reset(new Actor(relativePath));
        fileType = FILE_ACTOR;
    }
}

EngineFileType ProjectFile::getFileType()
{
    return fileType;
}

std::string ProjectFile::getRelativePath()
{
    return relativePath;
}

void ProjectFile::setTileSet(TileSet* tileSet)
{
	this->tileSet.reset(tileSet);
}

TileSet* ProjectFile::getTileSet()
{
    return tileSet.get();
}

void ProjectFile::setRoom(Room* room)
{
	this->room.reset(room);
}

Room* ProjectFile::getRoom()
{
    return room.get();
}

void ProjectFile::setActor(Actor* actor)
{
	this->actor.reset(actor);
}

Actor* ProjectFile::getActor()
{
	return actor.get();
}