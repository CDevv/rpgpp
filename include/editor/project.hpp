#ifndef _RPGPP_PROJECT_H
#define _RPGPP_PROJECT_H

#include "fileSystemService.hpp"
#include "gamedata.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class Project {
  private:
	std::string projectPath;
	std::string projectTitle;

  public:
	Project(const std::string &path);
	static void create(const std::string &dirPath, const std::string &title);
	json toJson();
	std::string &getTitle();
	std::string &getBasePath();
	std::vector<std::string> getPaths(EngineFileType fileType);
	GameData generateStruct();
	void runProject();
};

#endif