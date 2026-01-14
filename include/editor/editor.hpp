#ifndef _RPGPP_EDITOR_H
#define _RPGPP_EDITOR_H

#include "configurationFileService.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "resourceService.hpp"
#include <memory>

class Editor {
private:
  static Editor *instance_;
  static std::unique_ptr<EditorInterfaceService> ui;
  static std::unique_ptr<FileSystemService> fs;
  static std::unique_ptr<ResourceService> resources;
  static std::unique_ptr<ConfigurationFileService> configService;

public:
  Editor();
  static EditorInterfaceService &getUi();
  static FileSystemService &getFileSystem();
  static ResourceService &getResources();
  static ConfigurationFileService &getConfigService();
  void init();
  void update();
  void draw();
  void unload();
};

#endif
