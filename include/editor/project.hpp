#ifndef _RPGPP_EDITOR_PROJECT_H
#define _RPGPP_EDITOR_PROJECT_H

#include <string>
#include <vector>
#include "gamedata.hpp"

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
public:
    Project();
    Project(std::string filePath);
    GameData generateStruct();
    void generateCmdScript();
    std::string getProjectTitle();
    std::string getProjectBasePath();
    std::vector<std::string> getTileSetPaths();
    std::vector<std::string> getMapPaths();
    std::vector<std::string> getActorPaths();
};

#endif