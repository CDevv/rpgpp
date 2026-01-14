#include "editor/configurationFileService.hpp"
#include "ini.h"
#include <memory>

ConfigurationFileService::ConfigurationFileService() {
  // Read the configuration file.
  this->ini_file = std::make_unique<mINI::INIFile>(ini_file_path);
  this->ini_file->read(this->ini_structure);
}
