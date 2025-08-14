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