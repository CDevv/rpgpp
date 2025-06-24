#ifndef _RPGPP_EDITOR_PROJECT_H
#define _RPGPP_EDITOR_PROJECT_H

#include <string>
#include <vector>

class Project {
private:
    std::string projectBasePath;
    std::string projectPath;
    std::string projectTitle;
    std::string tileSetsPath;
    std::string mapsPath;
    std::vector<std::string> tileSetPathsList;
    std::vector<std::string> makeTileSetPaths();
    std::vector<std::string> mapPathsList;
    std::vector<std::string> makeMapPaths();
public:
    Project();
    Project(std::string filePath);
    std::string getProjectBasePath();
    std::vector<std::string> getTileSetPaths();
    std::vector<std::string> getMapPaths();
};

#endif