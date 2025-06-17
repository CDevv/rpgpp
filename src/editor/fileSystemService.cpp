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

FS_Result FileSystemService::openImage()
{
    FS_Result fsResult;

    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[1] = { { "Texture", "png" } };
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

    fsResult.result = result;
    if (result == NFD_OKAY) {
        fsResult.path = outPath;
        fsResult.fileName = GetFileName(outPath);

        NFD_FreePathU8(outPath);
    }

    return fsResult;
}


void FileSystemService::unload()
{
    NFD_Quit();
}

