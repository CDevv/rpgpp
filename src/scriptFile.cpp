
#include "scriptFile.h"

#include <nlohmann/json.hpp>

#include "raylib.h"

ScriptFile::ScriptFile(const std::string &path) { this->fileContents = LoadFileText(path.c_str()); }

nlohmann::json ScriptFile::dumpJson() { return nlohmann::json{}; }

std::string &ScriptFile::getFileContents() { return fileContents; }