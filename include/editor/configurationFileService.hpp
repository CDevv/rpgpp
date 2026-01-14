#ifndef _RPGPP_EDITOR_CONFIGURATIONFILESERVICE_H
#define _RPGPP_EDITOR_CONFIGURATIONFILESERVICE_H

#include "ini.h"
#include <memory>
constexpr const char *window_path_ref = "window";
constexpr const char *ini_file_path = "resources/rgpp.ini";
class ConfigurationFileService {
public:
  std::unique_ptr<mINI::INIFile> ini_file;
  mINI::INIStructure ini_structure;
  ConfigurationFileService();
};

#endif