#include "project.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

using json = nlohmann::json;

Project::Project(const std::string& path) {
    this->projectPath = path;

    char *jsonContent = LoadFileText(path.c_str());
    json j = json::parse(jsonContent);

    this->projectTitle = j.at("title");
}

std::string& Project::getTitle() {
    return projectTitle;
}