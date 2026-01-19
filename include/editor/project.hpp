#ifndef _RPGPP_PROJECT_H
#define _RPGPP_PROJECT_H

#include <string>

class Project {
private:
    std::string projectPath;
    std::string projectTitle;
public:
    Project(const std::string &path);
    std::string& getTitle();
};

#endif