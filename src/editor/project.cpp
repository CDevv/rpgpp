#include "project.hpp"
#include <filesystem>
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>

using json = nlohmann::json;

Project::Project(const std::string& path) {
    this->projectPath = GetDirectoryPath(path.c_str());

    char *jsonContent = LoadFileText(path.c_str());
    json j = json::parse(jsonContent);

    this->projectTitle = j.at("title");

    //ChangeDirectory(projectPath.c_str());
}

std::string& Project::getTitle() {
    return projectTitle;
}

std::vector<std::string> Project::getPaths() {
    std::filesystem::path subdir = projectPath;
    subdir /= "maps";
    auto pathList = LoadDirectoryFiles(subdir.c_str());
    std::vector<std::string> vec = {};
    
    for (int i = 0; i < pathList.count; i++) {
        vec.emplace_back(pathList.paths[i]);
    }

    UnloadDirectoryFiles(pathList);

    return vec;
}