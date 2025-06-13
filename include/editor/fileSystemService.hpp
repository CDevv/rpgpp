#ifndef _RPGPP_EDITOR_FILESYSTEMSERVICE_H
#define _RPGPP_EDITOR_FILESYSTEMSERVICE_H

#include <memory>
#include <string>
#include <raylib.h>
#include "tileset.hpp"

class FileSystemService {
private:
    std::string lastOpenPath;
    bool isOpen;
    std::unique_ptr<TileSet> lastTileSet;
public:
    FileSystemService();
    void unload();
    void promptOpenFile();
    bool fileIsOpen();
    std::string getOpenedFilePath();
    TileSet *getTileSet();
};

#endif
