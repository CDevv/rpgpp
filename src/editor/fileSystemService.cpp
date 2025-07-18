#include "fileSystemService.hpp"
#include "project.hpp"
#include "room.hpp"
#include <memory>
#include <nfd.h>
#include <raylib.h>

FileSystemService::FileSystemService()
{
    project = std::unique_ptr<Project>{};
    lastTileSet = std::unique_ptr<TileSet>{};
    lastRoom = std::unique_ptr<Room>{};
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

void FileSystemService::promptOpenProject()
{
    nfdu8filteritem_t filters[1] = { { "RPG++ Project", "rpgpp" } };
    FS_Result fsResult = openFile(filters);
    if (fsResult.result == NFD_OKAY) {
        project.reset(new Project(fsResult.path));
        SetWindowTitle(TextFormat("Editor - %s", project->getProjectTitle().c_str()));
    }
}

Project *FileSystemService::getProject()
{
    return project.get();
}

void FileSystemService::openProjectFile(std::string absolutePath)
{
    std::string fileExtension = GetFileExtension(absolutePath.c_str());
    if (TextIsEqual(fileExtension.c_str(), ".rtiles")) {
        lastTileSet.reset(new TileSet(absolutePath));
        lastType = FILE_TILESET;
    } else if (TextIsEqual(fileExtension.c_str(), ".rmap")) {
        lastRoom.reset(new Room(absolutePath));
        lastType = FILE_ROOM;
    }
    lastOpenPath = absolutePath;
    isOpen = true;
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
            lastRoom.reset(new Room(outPath));
            lastType = FILE_ROOM;
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

Room *FileSystemService::getRoom()
{
    return lastRoom.get();
}

FS_Result FileSystemService::openFile(nfdu8filteritem_t filters[])
{
    FS_Result fsResult;

    nfdu8char_t *outPath;
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

    fsResult.result = result;
    if (result == NFD_OKAY) {
        fsResult.absolutePath = outPath;
        fsResult.path = outPath;
        fsResult.fileName = GetFileName(outPath);

        if (project.get() != nullptr) {
            if (fsResult.path.find(project->getProjectBasePath()) != std::string::npos) {
                fsResult.path.erase(0, project->getProjectBasePath().size() + 1);
            }
        }

        NFD_FreePathU8(outPath);
    }

    return fsResult;
}

FS_Result FileSystemService::openImage()
{
    nfdu8filteritem_t filters[1] = { { "Texture", "png" } };
    return openFile(filters);
}

FS_Result FileSystemService::openTileSetResource()
{
    FS_Result fsResult;

    nfdu8filteritem_t filters[1] = { { "RPG++ TileSet", "rtiles" } };
    return openFile(filters);
}

