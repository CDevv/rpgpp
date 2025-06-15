#ifndef _RPGPP_EDITOR_FILESYSTEMSERVICE_H
#define _RPGPP_EDITOR_FILESYSTEMSERVICE_H

#include <memory>
#include <string>
#include <raylib.h>
#include "tileset.hpp"
#include "nfd.h"

struct FS_Result {
    std::string path;
    nfdresult_t result;
};

class FileSystemService {
private:
    std::string lastOpenPath;
    bool isOpen;
    std::unique_ptr<TileSet> lastTileSet;
public:
    FileSystemService();
    ~FileSystemService();
    void unload();
    void promptOpenFile();
    bool fileIsOpen();
    std::string getOpenedFilePath();
    TileSet *getTileSet();
    FS_Result openImage();
};

#endif
