#ifndef _RPGPP_EDITOR_PROJECT_H
#define _RPGPP_EDITOR_PROJECT_H

#include <string>
#include <vector>
#include <array>
#include "gamedata.hpp"
#include "projectFile.hpp"
#include "projectGenerator.hpp"
#include "projectCompiler.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Project {
private:
    ProjectGenerator projectGenerator;
    ProjectCompiler projectCompiler;
    std::string projectBasePath;
    std::string projectPath;
    std::string projectTitle;
    std::string tileSetsPath;
    std::string mapsPath;
    std::string actorsPath;
    std::string dialoguesPath;
    std::string imagesPath;
    std::string fontsPath;
    void generateCmdScript();
    void callCompiler();
    std::vector<std::string> makePaths(std::string dirPath);
    std::array<std::vector<std::string>, ENGINEFILETYPE_MAX> pathsList;
public:
    Project();
    Project(std::string filePath);
    void reloadPaths();
    static void generateNewProj(std::string title, std::string path);
    GameData generateStruct();
    void compileProject();
    void runProject();
    void cleanCompilation();
    std::string getProjectTitle();
    std::string getProjectBasePath();
    std::vector<std::string> getTypePaths(EngineFileType type);
};

#endif
