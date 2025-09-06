#ifndef _RPGPP_EDITOR_PROJECT_H
#define _RPGPP_EDITOR_PROJECT_H

#include <string>
#include <vector>
#include "gamedata.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Project {
private:
    std::string projectBasePath;
    std::string projectPath;
    std::string projectTitle;
    std::string tileSetsPath;
    std::string mapsPath;
    std::string actorsPath;
    std::vector<std::string> tileSetPathsList;
    std::vector<std::string> makeTileSetPaths();
    std::vector<std::string> mapPathsList;
    std::vector<std::string> makeMapPaths();
    std::vector<std::string> actorPathsList;
    std::vector<std::string> makeActorPaths();
    void generateCmdScript();
    void callCompiler();
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
    std::vector<std::string> getTileSetPaths();
    std::vector<std::string> getMapPaths();
    std::vector<std::string> getActorPaths();
};

#endif