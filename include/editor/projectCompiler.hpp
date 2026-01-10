#ifndef _RPGPP_EDITOR_PROJECTCOMPILER_H
#define _RPGPP_EDITOR_PROJECTCOMPILER_H

#include <string>
#include <vector>
#include <raylib.h>

class ProjectCompiler {
public:
	ProjectCompiler();
	void generateCmdScript(std::string projectTitle);
	void callCompiler();
	void cleanCompilation(const std::string &projectBase);
};

#endif