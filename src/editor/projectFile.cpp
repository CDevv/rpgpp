#include "projectFile.hpp"
#include "dialogueParser.hpp"
#include "tileset.hpp"
#include <memory>
#include <raylib.h>

#include "dialogueBalloon.hpp"
#include "tileset.hpp"
#include "room.hpp"
#include "actor.hpp"

std::array<std::string, 4> ProjectFile::fileTypeNames = std::array<std::string, 4>{
    "TileSet", "Map", "Actor", "Dialogues"
};

ProjectFile::ProjectFile() {}

ProjectFile::ProjectFile(std::string relativePath, EngineFileType fileType)
{
    this->relativePath = relativePath;
    this->fileType = fileType;

    this->variant = std::unique_ptr<VariantWrapper>{};
}

std::array<std::string, 4> ProjectFile::getTypeNames()
{
    return fileTypeNames;
}

void ProjectFile::setFromPath(std::string relativePath)
{
    std::string fileExtension = GetFileExtension(relativePath.c_str());
    if (TextIsEqual(fileExtension.c_str(), ".rtiles")) {
        setData(new TileSet(relativePath));
        fileType = FILE_TILESET;
    } else if (TextIsEqual(fileExtension.c_str(), ".rmap")) {
        setData(new Room(relativePath));
        fileType = FILE_ROOM;
    } else if (TextIsEqual(fileExtension.c_str(), ".ractor")) {
        setData(new Actor(relativePath));
        fileType = FILE_ACTOR;
    } else if (TextIsEqual(fileExtension.c_str(), ".rdiag")) {
        std::string fileText = LoadFileText(relativePath.c_str());
        Dialogue* diag = new Dialogue(parseDialogueText(fileText));
        diag->title = GetFileNameWithoutExt(relativePath.c_str());
        setData(diag);
        fileType = FILE_DIALOGUE;
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
