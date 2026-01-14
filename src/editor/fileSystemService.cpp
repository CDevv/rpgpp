#include "fileSystemService.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "project.hpp"
#include "projectFile.hpp"
#include "room.hpp"
#include <array>
#include <memory>
#include <nfd.h>
#include <raylib.h>

FileSystemService::FileSystemService() {
  project = std::unique_ptr<Project>{};
  projectAvailable = false;
  activeIndex = 0;
  lastType = FILE_ACTOR;

  lastTileSet = nullptr;
  lastRoom = nullptr;
  lastActor = nullptr;
  isOpen = false;

  NFD_Init();
}

FileSystemService::~FileSystemService() { NFD_Quit(); }

void FileSystemService::unload() { NFD_Quit(); }

void FileSystemService::promptOpenProject() {
  nfdu8filteritem_t filters[1] = {{"RPG++ Project", "rpgpp"}};
  FS_Result fsResult = openFile(filters);
  if (fsResult.result == NFD_OKAY) {
    project.reset(new Project(fsResult.path));
    projectAvailable = true;
    SetWindowTitle(
        TextFormat("Editor - %s", project->getProjectTitle().c_str()));
  }
}

void FileSystemService::setProject(const std::string &projectPath) {
  project.reset(new Project(projectPath));
  projectAvailable = true;
  SetWindowTitle(TextFormat("Editor - %s", project->getProjectTitle().c_str()));
}

Project *FileSystemService::getProject() { return project.get(); }

bool FileSystemService::projectIsOpen() { return projectAvailable; }

void FileSystemService::openProjectFile(std::string absolutePath) {
  bool exists = false;
  for (std::vector<std::unique_ptr<ProjectFile>>::iterator i =
           openedFiles.begin();
       i != openedFiles.end(); ++i) {
    if (i->get()->getRelativePath() == absolutePath) {
      exists = true;
      break;
    }
  }

  if (exists)
    return;

  EngineFileType fileType;

  std::unique_ptr<ProjectFile> projectFile =
      std::make_unique<ProjectFile>(absolutePath, fileType);
  projectFile->setFromPath(absolutePath);

  openedFiles.push_back(std::move(projectFile));
  setActiveProjectFile(openedFiles.size() - 1);

  // tabList
  EditorInterfaceService &ui = Editor::getUi();
  ui.getTabList().addItem(GetFileName(absolutePath.c_str()));
}

void FileSystemService::setActiveProjectFile(int index) {
  int currentIndex = 0;
  for (std::vector<std::unique_ptr<ProjectFile>>::iterator i =
           openedFiles.begin();
       i != openedFiles.end(); ++i) {
    if (currentIndex == index) {
      activeIndex = index;
      lastType = i->get()->getFileType();
      lastOpenPath = i->get()->getRelativePath();
      isOpen = true;

      break;
    }
    currentIndex++;
  }
}

void FileSystemService::closeProjectFile(int index) {
  int currentIndex = 0;
  for (std::vector<std::unique_ptr<ProjectFile>>::iterator i =
           openedFiles.begin();
       i != openedFiles.end(); ++i) {
    if (currentIndex == index) {
      i->reset();
      openedFiles.erase(i);
      if (index == 0) {
        isOpen = false;
      }

      break;
    }
    currentIndex++;
  }
}

void FileSystemService::promptOpenFile() {
  nfdu8char_t *outPath;
  nfdu8filteritem_t filters[1] = {{"RPG++ Resource", "rtiles,rmap"}};
  nfdopendialogu8args_t args = {nullptr};
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
    } else if (TextIsEqual(fileExtension.c_str(), ".ractor")) {
      lastActor = new Actor(outPath);
      lastType = FILE_ACTOR;
    }

    isOpen = true;

    NFD_FreePathU8(outPath);
  }
}

bool FileSystemService::fileIsOpen() { return isOpen; }

std::string FileSystemService::getOpenedFilePath() { return lastOpenPath; }

EngineFileType FileSystemService::getType() { return lastType; }

bool FileSystemService::isAvailable(EngineFileType type) {
  if (!fileIsOpen())
    return false;

  if (getCurrentFile() != nullptr) {
    if (getCurrentFile()->getFileType() == type) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

FS_Result FileSystemService::openFile(const nfdu8filteritem_t filters[]) {
  FS_Result fsResult;

  nfdu8char_t *outPath;
  nfdopendialogu8args_t args = {nullptr};
  args.filterList = filters;
  args.filterCount = 1;
  nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

  fsResult.result = result;
  if (result == NFD_OKAY) {
    fsResult.absolutePath = outPath;
    fsResult.path = outPath;
    fsResult.fileName = GetFileName(outPath);

    if (project.get() != nullptr) {
      if (fsResult.path.find(project->getProjectBasePath()) !=
          std::string::npos) {
        fsResult.path.erase(0, project->getProjectBasePath().size() + 1);
      }
    }

    NFD_FreePathU8(outPath);
  }

  return fsResult;
}

FS_Result FileSystemService::openFolder() {
  FS_Result fsResult;

  nfdu8char_t *outPath;
  nfdresult_t result = NFD_PickFolder(&outPath, nullptr);

  fsResult.result = result;

  if (result == NFD_OKAY) {
    fsResult.absolutePath = outPath;
    fsResult.path = outPath;

    NFD_FreePathU8(outPath);
  }

  return fsResult;
}

FS_Result FileSystemService::openGameData() {
  std::array<nfdu8filteritem_t, 1> filters = {{"RPG++ GameData", "bin"}};
  return openFile(filters.data());
}

FS_Result FileSystemService::openImage() {
  std::array<nfdu8filteritem_t, 1> filters = {{"Texture", "png"}};
  return openFile(filters.data());
}

FS_Result FileSystemService::openTileSetResource() {
  FS_Result fsResult;

  std::array<nfdu8filteritem_t, 1> filters = {{"RPG++ TileSet", "rtiles"}};
  return openFile(filters.data());
}

FS_Result FileSystemService::openDialogueResource() {
  FS_Result fsResult;

  std::array<nfdu8filteritem_t, 1> filters = {{"RPG++ Dialogue", "rdiag"}};
  return openFile(filters.data());
}

FS_Result FileSystemService::openThemeFile() {
  FS_Result fsResult;
  std::array<nfdu8filteritem_t, 1> filters = {{"TGUI Theme File", "txt"}};
  return openFile(filters.data());
}

FS_Result FileSystemService::openMusic() {
  FS_Result fsResult;

  std::array<nfdu8filteritem_t, 1> filters = {{"Music file", "ogg,wav"}};
  return openFile(filters.data());
}

ProjectFile *FileSystemService::getCurrentFile() {
  if (activeIndex >= openedFiles.size())
    return nullptr;
  return openedFiles.at(activeIndex).get();
}
