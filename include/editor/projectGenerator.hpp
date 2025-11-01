#ifndef _RPGPP_EDITOR_PROJECTGENERATOR_H
#define _RPGPP_EDITOR_PROJECTGENERATOR_H

#include <string>
#include <raylib.h>
#include <gamedata.hpp>
#include <projectFile.hpp>

class Project;

struct ProjectPaths {
    std::string tileSetsPath;
    std::string mapsPath;
    std::string actorsPath;
    std::string dialoguesPath;
    std::string imagesPath;
    std::string fontsPath;
    std::vector<std::string> tileSetPathsList;
    std::vector<std::string> mapPathsList;
    std::vector<std::string> actorPathsList;
    std::vector<std::string> dialoguesPathsList;
    std::vector<std::string> imagesPathsList;
    std::vector<std::string> fontsPathsList;
};

class ProjectGenerator {
public:
	ProjectGenerator();
	static void generateNewProj(std::string title, std::string path);
	GameData generateStruct(std::array<std::vector<std::string>, ENGINEFILETYPE_MAX>* proj,
	    std::string title);
};

#endif
