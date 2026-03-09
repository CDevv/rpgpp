
#include "scriptFile.h"

#include "raylib.h"

#include <nlohmann/json.hpp>

ScriptFile::ScriptFile(const std::string &path) {
	this->fileContents = LoadFileText(path.c_str());
}

nlohmann::json ScriptFile::dumpJson() { return nlohmann::json{}; }

std::string &ScriptFile::getFileContents() { return fileContents; }