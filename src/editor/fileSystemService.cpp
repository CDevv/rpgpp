#include "fileSystemService.hpp"
#include <nfd.h>

FileSystemService::FileSystemService()
{
    lastTileSet = std::unique_ptr<TileSet>{};
    lastTileMap = std::unique_ptr<TileMap>{};
    isOpen = false;

    NFD_Init();
}

FileSystemService::~FileSystemService()
{
    NFD_Quit();
}

void FileSystemService::unload()
{
    NFD_Quit();
}

void FileSystemService::promptOpenFile()
{
    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[1] = { { "RPG++ Resource", "rtiles,rmap" } };
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

    if (result == NFD_OKAY) {
        lastOpenPath = outPath;

        std::string fileExtension = GetFileExtension(outPath);
        if (TextIsEqual(fileExtension.c_str(), ".rtiles")) {
            lastTileSet.reset(new TileSet(outPath));
            lastType = FILE_TILESET;
        } else if (TextIsEqual(fileExtension.c_str(), ".rmap")) {
            lastTileMap.reset(new TileMap(outPath));
            lastType = FILE_MAP;
        }

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

EngineFileType FileSystemService::getType()
{
    return lastType;
}

TileSet *FileSystemService::getTileSet()
{
    return lastTileSet.get();
}

TileMap *FileSystemService::getTileMap()
{
    return lastTileMap.get();
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

FS_Result FileSystemService::openTileSetResource()
{
    FS_Result fsResult;

    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[1] = { { "RPG++ TIleSet", "rtiles" } };
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

