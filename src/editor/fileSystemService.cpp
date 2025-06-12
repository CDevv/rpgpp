#include "fileSystemService.hpp"
#include <nfd.h>

FileSystemService::FileSystemService()
{
    lastTileSet = nullptr;
    isOpen = false;

    NFD_Init();
}

void FileSystemService::promptOpenFile()
{
    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[1] = { { "RPG++ JSON Resource", "json" } };
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

    if (result == NFD_OKAY) {
        if (lastTileSet != nullptr) {
            lastTileSet->unload();
            delete lastTileSet;
        }

        lastOpenPath = outPath;
        lastTileSet = new TileSet(outPath);
        isOpen = true;
    }
}

bool FileSystemService::fileIsOpen()
{
    return isOpen;
}

std::string FileSystemService::getOpenedFilePath()
{
    return lastOpenPath;
}

TileSet *FileSystemService::getTileSet()
{
    return lastTileSet;
}

void FileSystemService::unload()
{
    if (isOpen && lastTileSet != nullptr) {
        lastTileSet->unload();
        delete lastTileSet;
    }

    NFD_Quit();
}

