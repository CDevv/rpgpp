#ifndef _RPGPP_EDITOR_FILESYSTEMSERVICE_H
#define _RPGPP_EDITOR_FILESYSTEMSERVICE_H

#include <memory>
#include <string>
#include <raylib.h>
#include "project.hpp"
#include "room.hpp"
#include "tileset.hpp"
#include "nfd.h"

enum EngineFileType {
    FILE_TILESET,
    FILE_ROOM
};

struct FS_Result {
    std::string absolutePath;
    std::string path;
    std::string fileName;
    nfdresult_t result;
};

class ProjectFile {
private:
    std::string relativePath;
    EngineFileType fileType;
    std::unique_ptr<TileSet> tileSet;
    std::unique_ptr<Room> room;
public:
    ProjectFile();
    ProjectFile(std::string relativePath, EngineFileType fileType, TileSet* tileSet, Room* room);
    std::string getRelativePath();
    EngineFileType getFileType();
    TileSet* getTileSet();
    Room* getRoom();
};

class FileSystemService {
private:
    bool projectAvailable;
    std::unique_ptr<Project> project;
    std::vector<std::unique_ptr<ProjectFile>> openedFiles;
    int activeIndex;
    EngineFileType lastType;
    std::string lastOpenPath;
    bool isOpen;
    TileSet* lastTileSet;
    Room* lastRoom;
public:
    FileSystemService();
    ~FileSystemService();
    void unload();
    void promptOpenProject();
    Project *getProject();
    bool projectIsOpen();
    void openProjectFile(std::string absolutePath);
    void setActiveProjectFile(int index);
    void closeProjectFile(int index);
    void promptOpenFile();
    void saveOpenedFile();
    bool fileIsOpen();
    std::string getOpenedFilePath();
    EngineFileType getType();
    TileSet *getTileSet();
    Room *getRoom();
    FS_Result openFile(nfdu8filteritem_t filters[]);
    FS_Result openGameData();
    FS_Result openImage();
    FS_Result openTileSetResource();
};

#endif
