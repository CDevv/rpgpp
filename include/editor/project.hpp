#ifndef _RPGPP_PROJECT_H
#define _RPGPP_PROJECT_H

#include <string>
#include <vector>

class Project {
private:
    std::string projectPath;
    std::string projectTitle;
public:
    Project(const std::string &path);
    std::string& getTitle();
    std::vector<std::string> getPaths();
};

#endif