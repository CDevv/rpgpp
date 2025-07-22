#include "fileSystemService.hpp"
#include "project.hpp"
#include "room.hpp"
#include <memory>
#include <nfd.h>
#include <raylib.h>

ProjectFile::ProjectFile() {}

ProjectFile::ProjectFile(std::string relativePath, EngineFileType fileType, TileSet* tileSet, Room* room)
{
    this->relativePath = relativePath;
    this->fileType = fileType;
    this->tileSet.reset(tileSet);
    this->room.reset(room);
}

EngineFileType ProjectFile::getFileType()
{
    return fileType;
}

std::string ProjectFile::getRelativePath()
{
    return relativePath;
}

TileSet* ProjectFile::getTileSet()
{
    return tileSet.get();
}

Room* ProjectFile::getRoom()
{
    return room.get();
}

FileSystemService::FileSystemService()
{
    project = std::unique_ptr<Project>{};
    activeIndex = 0;

    //lastTileSet = std::unique_ptr<TileSet>{};
    //lastRoom = std::unique_ptr<Room>{};
    lastTileSet = nullptr;
    lastRoom = nullptr;
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
    EngineFileType fileType;
    TileSet* tileSet = nullptr;
    Room* room = nullptr;    

    std::string fileExtension = GetFileExtension(absolutePath.c_str());
    if (TextIsEqual(fileExtension.c_str(), ".rtiles")) {
        tileSet = new TileSet(absolutePath);
        fileType = FILE_TILESET;
    } else if (TextIsEqual(fileExtension.c_str(), ".rmap")) {
        room = new Room(absolutePath);
        fileType = FILE_ROOM;
    }

    std::unique_ptr<ProjectFile> projectFile = std::make_unique<ProjectFile>(absolutePath, fileType, tileSet, room);
    openedFiles.push_back(std::move(projectFile));
    setActiveProjectFile(openedFiles.size() - 1);
}

void FileSystemService::setActiveProjectFile(int index)
{
    int currentIndex = 0;
    for (std::vector<std::unique_ptr<ProjectFile>>::iterator i = openedFiles.begin(); i != openedFiles.end(); ++i)
    {
        if (currentIndex == index) {
            activeIndex = index;
            lastType = i->get()->getFileType();
            switch (lastType) {
            case FILE_TILESET:
                lastTileSet = i->get()->getTileSet();
                break;
            case FILE_ROOM:
                lastRoom = i->get()->getRoom();
                break;
            }
            lastOpenPath = i->get()->getRelativePath();
            isOpen = true;

            break;
        }
        currentIndex++;
    }
}

void FileSystemService::closeProjectFile(int index)
{
    int currentIndex = 0;
    for (std::vector<std::unique_ptr<ProjectFile>>::iterator i = openedFiles.begin(); i != openedFiles.end(); ++i)
    {
        if (currentIndex == index) {
            openedFiles.erase(i);

            break;
        }
        currentIndex++;
    }
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
            lastTileSet = new TileSet(outPath);
            lastType = FILE_TILESET;
        } else if (TextIsEqual(fileExtension.c_str(), ".rmap")) {
            lastRoom = new Room(outPath);
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
    return lastTileSet;
}

Room *FileSystemService::getRoom()
{
    return lastRoom;
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

