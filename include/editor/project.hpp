#ifndef _RPGPP_PROJECT_H
#define _RPGPP_PROJECT_H

#include "fileSystemService.hpp"
#include <string>
#include <vector>

class Project {
  private:
	std::string projectPath;
	std::string projectTitle;

  public:
	Project(const std::string &path);
	std::string &getTitle();
	std::string &getBasePath();
	std::vector<std::string> getPaths(EngineFileType fileType);
	void runProject();
};

#endif