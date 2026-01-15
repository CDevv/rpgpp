#include "editor/configurationFileService.hpp"
#include "ini.h"
#include "raylib.h"
#include <memory>

ConfigurationFileService::ConfigurationFileService() {
  // Read the configuration file.
  this->ini_file = std::make_unique<mINI::INIFile>(ini_file_path);
  this->ini_file->read(this->ini_structure);

  auto map_ref = ini_structure["window"]["theme_file"];
  if (!FileExists(map_ref.c_str())) {
    //Theme .txt file does not exist
    const char* themeSource = "resources/themes/RPG2007.txt";
    ini_structure["window"]["theme_file"] = themeSource;
    ini_file->write(ini_structure);
  }
}
