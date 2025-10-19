#ifndef _RPGPP_EDITOR_PROJECTGENERATOR_H
#define _RPGPP_EDITOR_PROJECTGENERATOR_H

#include <string>
#include <raylib.h>
#include <gamedata.hpp>

class Project;

struct ProjectPaths {
    std::string tileSetsPath;
    std::string mapsPath;
    std::string actorsPath;
    std::string dialoguesPath;
    std::vector<std::string> tileSetPathsList;
    std::vector<std::string> mapPathsList;
    std::vector<std::string> actorPathsList;
    std::vector<std::string> dialoguesPathsList;
};

class ProjectGenerator {
public:
	ProjectGenerator();
	static void generateNewProj(std::string title, std::string path);
	GameData generateStruct(ProjectPaths proj, std::string title);
};

#endif
