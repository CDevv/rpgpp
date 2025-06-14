#include "fileSystemService.hpp"
#include <nfd.h>

FileSystemService::FileSystemService()
{
    lastTileSet = std::unique_ptr<TileSet>{};
    isOpen = false;

    NFD_Init();
}

FileSystemService::~FileSystemService()
{
    NFD_Quit();
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
        lastOpenPath = outPath;
        lastTileSet.reset(new TileSet(outPath));
        isOpen = true;

        NFD_FreePathU8(outPath);
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
    return lastTileSet.get();
}

void FileSystemService::unload()
{
    NFD_Quit();
}

