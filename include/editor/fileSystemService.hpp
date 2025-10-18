#ifndef _RPGPP_EDITOR_FILESYSTEMSERVICE_H
#define _RPGPP_EDITOR_FILESYSTEMSERVICE_H

#include <memory>
#include <string>
#include <raylib.h>
#include "projectFile.hpp"
#include "project.hpp"
#include "room.hpp"
#include "tileset.hpp"
#include "actor.hpp"
#include "nfd.h"

struct FS_Result {
    std::string absolutePath;
    std::string path;
    std::string fileName;
    nfdresult_t result;
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
    Actor* lastActor;
public:
    FileSystemService();
    ~FileSystemService();
    void unload();
    void promptOpenProject();
    void setProject(std::string projectPath);
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
    bool isAvailable(EngineFileType type);
    ProjectFile* getCurrentFile();
    FS_Result openFile(nfdu8filteritem_t filters[]);
    FS_Result openFolder();
    FS_Result openGameData();
    FS_Result openImage();
    FS_Result openTileSetResource();
};

#endif
