#ifndef _RPGPP_EDITOR_FILESYSTEMSERVICE_H
#define _RPGPP_EDITOR_FILESYSTEMSERVICE_H

#include <memory>
#include <string>
#include <raylib.h>
#include "project.hpp"
#include "tileset.hpp"
#include "tilemap.hpp"
#include "nfd.h"

enum EngineFileType {
    FILE_TILESET,
    FILE_MAP
};

struct FS_Result {
    std::string path;
    std::string fileName;
    nfdresult_t result;
};

class FileSystemService {
private:
    std::unique_ptr<Project> project;
    EngineFileType lastType;
    std::string lastOpenPath;
    bool isOpen;
    std::unique_ptr<TileSet> lastTileSet;
    std::unique_ptr<TileMap> lastTileMap;
public:
    FileSystemService();
    ~FileSystemService();
    void unload();
    void promptOpenProject();
    Project *getProject();
    void openProjectFile(std::string absolutePath);
    void promptOpenFile();
    void saveOpenedFile();
    bool fileIsOpen();
    std::string getOpenedFilePath();
    EngineFileType getType();
    TileSet *getTileSet();
    TileMap *getTileMap();
    FS_Result openFile(nfdu8filteritem_t filters[]);
    FS_Result openImage();
    FS_Result openTileSetResource();
};

#endif
